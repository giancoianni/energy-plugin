#include <gz/sim/System.hh>
#include <gz/plugin/Register.hh>
#include <gz/common/Console.hh>

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
    }
  };
}

GZ_ADD_PLUGIN(
  thesis::EnergyPlugin,
  gz::sim::System,
  thesis::EnergyPlugin::ISystemConfigure,
  thesis::EnergyPlugin::ISystemPreUpdate
)