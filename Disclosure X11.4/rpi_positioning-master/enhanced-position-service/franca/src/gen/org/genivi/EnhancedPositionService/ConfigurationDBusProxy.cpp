/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.core 2.1.6.v20140519.
* Used org.franca.core 0.8.11.201401091023.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
/**
 * Configuration = This interface allows a client application to set and retrieve
 *  configuration options
 */
#include "ConfigurationDBusProxy.h"

namespace org {
namespace genivi {
namespace EnhancedPositionService {

std::shared_ptr<CommonAPI::DBus::DBusProxy> createConfigurationDBusProxy(
                    const std::shared_ptr<CommonAPI::DBus::DBusFactory>& factory,
                    const std::string& commonApiAddress,
                    const std::string& interfaceName,
                    const std::string& busName,
                    const std::string& objectPath,
                    const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection>& dbusProxyConnection) {
    return std::make_shared<ConfigurationDBusProxy>(factory, commonApiAddress, interfaceName, busName, objectPath, dbusProxyConnection);
}

INITIALIZER(registerConfigurationDBusProxy) {
    CommonAPI::DBus::DBusFactory::registerProxyFactoryMethod(Configuration::getInterfaceId(),
       &createConfigurationDBusProxy);
}

ConfigurationDBusProxy::ConfigurationDBusProxy(
                    const std::shared_ptr<CommonAPI::DBus::DBusFactory>& factory,
                    const std::string& commonApiAddress,
                    const std::string& interfaceName,
                    const std::string& busName,
                    const std::string& objectPath,
                    const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection>& dbusProxyconnection):
        CommonAPI::DBus::DBusProxy(factory, commonApiAddress, interfaceName, busName, objectPath, dbusProxyconnection)
,satSystem_(*this, "onSatSystemAttributeChanged", "setSatSystemAttribute", "i", "getSatSystemAttribute"),
updateInterval_(*this, "onUpdateIntervalAttributeChanged", "setUpdateIntervalAttribute", "i", "getUpdateIntervalAttribute")
    {
    }

ConfigurationDBusProxy::SatSystemAttribute& ConfigurationDBusProxy::getSatSystemAttribute() {
    return satSystem_;
}
ConfigurationDBusProxy::UpdateIntervalAttribute& ConfigurationDBusProxy::getUpdateIntervalAttribute() {
    return updateInterval_;
}


/**
 * GetVersion = This method returns the API version implemented by the server
 *  application
 */
void ConfigurationDBusProxy::GetVersion(CommonAPI::CallStatus& callStatus, EnhancedPositionServiceTypes::Version& version) {
    CommonAPI::DBus::DBusProxyHelper<CommonAPI::DBus::DBusSerializableArguments<>,
                                     CommonAPI::DBus::DBusSerializableArguments<EnhancedPositionServiceTypes::Version> >::callMethodWithReply(
        *this,
        "GetVersion",
        "",
        callStatus
        , version);
}
std::future<CommonAPI::CallStatus> ConfigurationDBusProxy::GetVersionAsync(GetVersionAsyncCallback callback) {
    return CommonAPI::DBus::DBusProxyHelper<CommonAPI::DBus::DBusSerializableArguments<>,
                                     CommonAPI::DBus::DBusSerializableArguments<EnhancedPositionServiceTypes::Version> >::callMethodAsync(
        *this,
        "GetVersion",
        "",
        std::move(callback));
}
/**
 * GetSupportedSatelliteSystems = This method returns a list of supported
 *  satellite systems
 */
void ConfigurationDBusProxy::GetSupportedSatelliteSystems(CommonAPI::CallStatus& callStatus, std::vector<EnhancedPositionServiceTypes::SatelliteSystem>& satelliteSystems) {
    CommonAPI::DBus::DBusProxyHelper<CommonAPI::DBus::DBusSerializableArguments<>,
                                     CommonAPI::DBus::DBusSerializableArguments<std::vector<EnhancedPositionServiceTypes::SatelliteSystem>> >::callMethodWithReply(
        *this,
        "GetSupportedSatelliteSystems",
        "",
        callStatus
        , satelliteSystems);
}
std::future<CommonAPI::CallStatus> ConfigurationDBusProxy::GetSupportedSatelliteSystemsAsync(GetSupportedSatelliteSystemsAsyncCallback callback) {
    return CommonAPI::DBus::DBusProxyHelper<CommonAPI::DBus::DBusSerializableArguments<>,
                                     CommonAPI::DBus::DBusSerializableArguments<std::vector<EnhancedPositionServiceTypes::SatelliteSystem>> >::callMethodAsync(
        *this,
        "GetSupportedSatelliteSystems",
        "",
        std::move(callback));
}



void ConfigurationDBusProxy::getOwnVersion(uint16_t& ownVersionMajor, uint16_t& ownVersionMinor) const {
    ownVersionMajor = 0;
    ownVersionMinor = 0;
}

} // namespace EnhancedPositionService
} // namespace genivi
} // namespace org
