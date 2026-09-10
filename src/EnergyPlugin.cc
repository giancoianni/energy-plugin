#include <gz/sim/System.hh>
#include <gz/plugin/Register.hh>
#include <gz/common/Console.hh>
#include <gz/sim/components/Name.hh>
#include <gz/sim/components/Model.hh>
#include <gz/sim/components/Pose.hh>

#include <chrono>

namespace thesis
{
  class EnergyPlugin:
    public gz::sim::System,
    public gz::sim::ISystemConfigure,
    public gz::sim::ISystemPreUpdate
  {
    public: void Configure(
      const gz::sim::Entity &_entity,
      const std::shared_ptr<const sdf::Element> &_sdf,
      gz::sim::EntityComponentManager &_ecm,
      gz::sim::EventManager &_eventMgr) override
    {
      gzmsg << "[EnergyPlugin] Loaded successfully." << std::endl;
      gzmsg << "[EnergyPlugin] Attached to entity: "
            << _entity << std::endl;
    }

    public: void PreUpdate(
      const gz::sim::UpdateInfo &_info,
      gz::sim::EntityComponentManager &_ecm) override
    {
      // Print all named entities once so we can inspect
      // what Gazebo has stored in the ECM.
      if (!this->entitiesPrinted)
      {
        gzmsg << "[EnergyPlugin] Listing named entities:" << std::endl;

        _ecm.Each<gz::sim::components::Name>(
          [&](const gz::sim::Entity &_entity,
              const gz::sim::components::Name *_name) -> bool
          {
            gzmsg << "  Entity " << _entity
                  << " | Name: " << _name->Data()
                  << std::endl;

            return true;
          });

        this->entitiesPrinted = true;
      }

      // Try to find test_box if it hasn't been found yet.
      if (this->boxEntity == gz::sim::kNullEntity)
      {
        this->boxEntity = _ecm.EntityByComponents(
          gz::sim::components::Name("test_box"));

        if (this->boxEntity != gz::sim::kNullEntity)
        {
          gzmsg << "[EnergyPlugin] Found test_box with entity ID: "
                << this->boxEntity << std::endl;
        }
      }

      // Once test_box has been found, print its pose once.
      if (this->boxEntity != gz::sim::kNullEntity &&
          !this->boxPosePrinted)
      {
        const auto poseComp =
          _ecm.Component<gz::sim::components::Pose>(
            this->boxEntity);

        if (poseComp)
        {
          const auto &pose = poseComp->Data();

          gzmsg << "[EnergyPlugin] test_box position: "
                << "x=" << pose.Pos().X()
                << ", y=" << pose.Pos().Y()
                << ", z=" << pose.Pos().Z()
                << std::endl;

          this->boxPosePrinted = true;
        }
      }

      // Do not accumulate energy while the simulation is paused.
      if (_info.paused)
      {
        return;
      }

      // Convert Gazebo's timestep into seconds.
      const double dt =
        std::chrono::duration<double>(_info.dt).count();

      // Temporary constant power model: 10 watts.
      const double powerWatts = 10.0;

      // Energy consumed during this timestep.
      this->totalEnergyJoules += powerWatts * dt;

      // Current simulation time in seconds.
      const double simTime =
        std::chrono::duration<double>(_info.simTime).count();

      // Print approximately once per simulated second.
      if (simTime >= this->nextPrintTime)
      {
        gzmsg << "[EnergyPlugin] Time: " << simTime
              << " s | Power: " << powerWatts
              << " W | Energy: " << this->totalEnergyJoules
              << " J"
              << std::endl;

        this->nextPrintTime += 1.0;
      }
    }

    private: gz::sim::Entity boxEntity = gz::sim::kNullEntity;

    private: bool boxPosePrinted = false;

    // Temporary debugging flag.
    private: bool entitiesPrinted = false;

    private: double totalEnergyJoules = 0.0;

    private: double nextPrintTime = 1.0;
  };
}

GZ_ADD_PLUGIN(
  thesis::EnergyPlugin,
  gz::sim::System,
  thesis::EnergyPlugin::ISystemConfigure,
  thesis::EnergyPlugin::ISystemPreUpdate
)