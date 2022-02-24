#ifndef _BLEFINGERPRINTCOLLECTION_
#define _BLEFINGERPRINTCOLLECTION_

#include "BleFingerprint.h"
#include <ArduinoJson.h>

#define ONE_EURO_FCMIN 1e-5f
#define ONE_EURO_BETA 1e-7f
#define ONE_EURO_DCUTOFF 1e-5f

class BleFingerprintCollection : public BLEAdvertisedDeviceCallbacks
{
public:
    BleFingerprintCollection()
    {
        fingerprintSemaphore = xSemaphoreCreateBinary();
        xSemaphoreGive(fingerprintSemaphore);
    }
    BleFingerprint *getFingerprint(BLEAdvertisedDevice *advertisedDevice);
    void cleanupOldFingerprints();
    std::list<BleFingerprint *> getCopy();
    void setDisable(bool disable) { _disable = disable; }

    static String include, exclude, query;
    static float skipDistance, maxDistance;
    static int refRssi, forgetMs, skipMs;

private:
    bool _disable = false;

    unsigned long lastCleanup = 0;

    SemaphoreHandle_t fingerprintSemaphore;
    std::list<BleFingerprint *> fingerprints;
    BleFingerprint *getFingerprintInternal(BLEAdvertisedDevice *advertisedDevice);

    void onResult(BLEAdvertisedDevice *advertisedDevice) override
    {
        if (_disable) return;

        GUI::seenStart();
        BleFingerprint *f = getFingerprint(advertisedDevice);
        if (f->seen(advertisedDevice))
            GUI::added(f);
        GUI::seenEnd();
    }
};

#endif
