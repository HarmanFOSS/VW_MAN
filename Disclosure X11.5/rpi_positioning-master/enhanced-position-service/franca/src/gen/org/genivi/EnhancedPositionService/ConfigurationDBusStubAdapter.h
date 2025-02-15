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
#ifndef ORG_GENIVI_ENHANCEDPOSITIONSERVICE_Configuration_DBUS_STUB_ADAPTER_H_
#define ORG_GENIVI_ENHANCEDPOSITIONSERVICE_Configuration_DBUS_STUB_ADAPTER_H_

#include <org/genivi/EnhancedPositionService/ConfigurationStub.h>

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/DBus/DBusStubAdapterHelper.h>
#include <CommonAPI/DBus/DBusStubAdapter.h>
#include <CommonAPI/DBus/DBusFactory.h>
#include <CommonAPI/DBus/DBusServicePublisher.h>

#undef COMMONAPI_INTERNAL_COMPILATION

namespace org {
namespace genivi {
namespace EnhancedPositionService {

typedef CommonAPI::DBus::DBusStubAdapterHelper<ConfigurationStub> ConfigurationDBusStubAdapterHelper;

class ConfigurationDBusStubAdapterInternal: public virtual ConfigurationStubAdapter, public ConfigurationDBusStubAdapterHelper {
 public:
    ConfigurationDBusStubAdapterInternal(
            const std::shared_ptr<CommonAPI::DBus::DBusFactory>& factory,
            const std::string& commonApiAddress,
            const std::string& dbusInterfaceName,
            const std::string& dbusBusName,
            const std::string& dbusObjectPath,
            const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection>& dbusConnection,
            const std::shared_ptr<CommonAPI::StubBase>& stub);

    ~ConfigurationDBusStubAdapterInternal();

    virtual const bool hasFreedesktopProperties();

    /**
     * SatSystem = satellite system (GPS, GLONASS, ...)
     */
    void fireSatSystemAttributeChanged(const EnhancedPositionServiceTypes::SatelliteSystem& value);
    /**
     * UpdateInterval = update interval
     */
    void fireUpdateIntervalAttributeChanged(const int32_t& value);



    const ConfigurationDBusStubAdapterHelper::StubDispatcherTable& getStubDispatcherTable();
    const CommonAPI::DBus::StubAttributeTable& getStubAttributeTable();

    void deactivateManagedInstances();


static CommonAPI::DBus::DBusGetAttributeStubDispatcher<
        ConfigurationStub,
        CommonAPI::Version
        > getConfigurationInterfaceVersionStubDispatcher;

/**
 * SatSystem = satellite system (GPS, GLONASS, ...)
 */
static CommonAPI::DBus::DBusGetAttributeStubDispatcher<
        ConfigurationStub,
        EnhancedPositionServiceTypes::SatelliteSystem
        > getSatSystemAttributeStubDispatcher;
static CommonAPI::DBus::DBusSetObservableAttributeStubDispatcher<
        ConfigurationStub,
        EnhancedPositionServiceTypes::SatelliteSystem
        > setSatSystemAttributeStubDispatcher;
/**
 * UpdateInterval = update interval
 */
static CommonAPI::DBus::DBusGetAttributeStubDispatcher<
        ConfigurationStub,
        int32_t
        > getUpdateIntervalAttributeStubDispatcher;
static CommonAPI::DBus::DBusSetObservableAttributeStubDispatcher<
        ConfigurationStub,
        int32_t
        > setUpdateIntervalAttributeStubDispatcher;


/**
 * GetVersion = This method returns the API version implemented by the server
 *  application
 */
static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ConfigurationStub,
    std::tuple<>,
    std::tuple<EnhancedPositionServiceTypes::Version>
    > getVersionStubDispatcher;
/**
 * GetSupportedSatelliteSystems = This method returns a list of supported
 *  satellite systems
 */
static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ConfigurationStub,
    std::tuple<>,
    std::tuple<std::vector<EnhancedPositionServiceTypes::SatelliteSystem>>
    > getSupportedSatelliteSystemsStubDispatcher;




 protected:
    virtual const char* getMethodsDBusIntrospectionXmlData() const;

 private:
    ConfigurationDBusStubAdapterHelper::StubDispatcherTable stubDispatcherTable_;
    CommonAPI::DBus::StubAttributeTable stubAttributeTable_;
};

class ConfigurationDBusStubAdapter: public ConfigurationDBusStubAdapterInternal, public std::enable_shared_from_this<ConfigurationDBusStubAdapter> {
public:
    ConfigurationDBusStubAdapter(
                         const std::shared_ptr<CommonAPI::DBus::DBusFactory>& factory,
                         const std::string& commonApiAddress,
                         const std::string& dbusInterfaceName,
                         const std::string& dbusBusName,
                         const std::string& dbusObjectPath,
                         const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection>& dbusConnection,
                         const std::shared_ptr<CommonAPI::StubBase>& stub) :
            CommonAPI::DBus::DBusStubAdapter(
                            factory,
                            commonApiAddress,
                            dbusInterfaceName,
                            dbusBusName,
                            dbusObjectPath,
                            dbusConnection,
                            false),
            ConfigurationDBusStubAdapterInternal(
                            factory,
                            commonApiAddress,
                            dbusInterfaceName,
                            dbusBusName,
                            dbusObjectPath,
                            dbusConnection,
                            stub) { }
};

} // namespace EnhancedPositionService
} // namespace genivi
} // namespace org

#endif // ORG_GENIVI_ENHANCEDPOSITIONSERVICE_Configuration_DBUS_STUB_ADAPTER_H_
