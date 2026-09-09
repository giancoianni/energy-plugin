#include <gz/sim/System.hh>
#include <gz/plugin/Register.hh>
#include <gz/common/Console.hh>

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
      gzmsg << "[EnergyPlugin] Attached to entity: " << _entity << std::endl;
    }

    public: void PreUpdate(
      const gz::sim::UpdateInfo &_info,
      gz::sim::EntityComponentManager &_ecm) override
    {
      if (_info.paused)
      {
        return;
      }

      // Convert Gazebo's timestep into seconds
      const double dt =
        std::chrono::duration<double>(_info.dt).count();

      // Temporary constant power model: 10 watts
      const double powerWatts = 10.0;

      // Energy consumed during this timestep
      this->totalEnergyJoules += powerWatts * dt;

      // Current simulation time in seconds
      const double simTime =
        std::chrono::duration<double>(_info.simTime).count();

      // Print approximately once per simulated second
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