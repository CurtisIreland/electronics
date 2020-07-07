/*
 * BLE2901.cpp
 *
 */

/*
 * See also:
 * https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
 */
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "BLE2901.h"

BLE2901::BLE2901() : BLEDescriptor(BLEUUID((uint16_t) 0x2901)) {
  uint8_t data[2] = { 0, 0 };
  setValue(data, 2);
} // BLE2901


/**
 * @brief Get the notifications value.
 * @return The notifications value.  True if notifications are enabled and false if not.
 */
bool BLE2901::getNotifications() {
  return (getValue()[0] & (1 << 0)) != 0;
} // getNotifications


/**
 * @brief Get the indications value.
 * @return The indications value.  True if indications are enabled and false if not.
 */
bool BLE2901::getIndications() {
  return (getValue()[0] & (1 << 1)) != 0;
} // getIndications


/**
 * @brief Set the indications flag.
 * @param [in] flag The indications flag.
 */
void BLE2901::setIndications(bool flag) {
  uint8_t *pValue = getValue();
  if (flag) pValue[0] |= 1 << 1;
  else pValue[0] &= ~(1 << 1);
} // setIndications


/**
 * @brief Set the notifications flag.
 * @param [in] flag The notifications flag.
 */
void BLE2901::setNotifications(bool flag) {
  uint8_t *pValue = getValue();
  if (flag) pValue[0] |= 1 << 0;
  else pValue[0] &= ~(1 << 0);
} // setNotifications

#endif
