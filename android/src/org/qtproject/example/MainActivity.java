package org.qtproject.example;

import org.qtproject.qt.android.bindings.QtActivity;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.WindowManager;
import android.view.KeyEvent;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.util.Log;
import android.view.View;
import android.content.Intent;
import java.io.IOException;
import java.io.DataOutputStream;

import android.content.Intent;
import android.provider.Settings;
import android.os.Bundle;

import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;

import java.util.HashMap;
import java.util.List;       // 导入List接口
import java.util.ArrayList;  // 导入ArrayList类，List接口的一个常见实现类
import java.util.Iterator;    // 导入Iterator接口



public class MainActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {        
        super.onCreate(savedInstanceState);

        setView();


        //屏幕高亮
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        //沉浸模式
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
        getWindow().getDecorView().setFitsSystemWindows(true);

    }

    public void setView() {
        UsbManager usbManager = (UsbManager) getSystemService(Context.USB_SERVICE);
        HashMap<String, UsbDevice> deviceList = usbManager.getDeviceList();
        Iterator<UsbDevice> deviceIterator = deviceList.values().iterator();
        List<UsbDevice> usbDevices = new ArrayList<>();
        while (deviceIterator.hasNext()) {
            UsbDevice device = deviceIterator.next();
            usbDevices.add(device);
            Log.e("getDeviceList", "getDeviceList: " + device.getProductName());
        }
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if(keyCode == KeyEvent.KEYCODE_BACK){
            return true;
        }
        return super.onKeyDown(keyCode, event);
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        if(keyCode == KeyEvent.KEYCODE_BACK){
            return true;
        }
        return super.onKeyUp(keyCode, event);
    }




}
