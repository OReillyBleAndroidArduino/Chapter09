package tonyg.example.com.exampleblescan;

import android.app.Application;

import tonyg.example.com.exampleblescan.ble.BlePeripheral;

/**
 * This class holds the Bluetooth device connections between activities
 *
 * @author Tony Gaitatzis backupbrain@gmail.com
 * @date 2016-03-05
 */
public class BleApplication extends Application {

    private BlePeripheral blePeripheral;

    @Override
    public void onCreate() {
        super.onCreate();
    }

    public void setBlePeripheral(BlePeripheral blePeripheral) {
        this.blePeripheral = blePeripheral;
    }
    public BlePeripheral getBlePeripheral() {
        return blePeripheral;
    }
}
