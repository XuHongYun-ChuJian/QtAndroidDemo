package org.qtproject.example.utils;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.util.Log;
import android.view.View;
import android.content.Intent;
import java.io.IOException;
import java.lang.reflect.Method;
import java.io.DataOutputStream;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import android.os.Environment;
import android.util.Log;

import android.content.Intent;
import android.provider.Settings;
import android.os.Bundle;

import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.os.storage.StorageManager;
import android.os.storage.StorageVolume;

import java.util.HashMap;
import java.util.List; // 导入List接口
import java.util.ArrayList; // 导入ArrayList类，List接口的一个常见实现类
import java.util.Iterator; // 导入Iterator接口

public class JavaUtils {
    // 请求文件访问权限的请求码，可以是任意整数值
    private static final int REQUEST_MANAGE_FILES_ACCESS = 2;
    static final private String TAG = "JavaUtils";
    static private JavaUtils mInstance = new JavaUtils();
    private Context mContext = null;

    private JavaUtils() {
    }

    static public JavaUtils instance() {
        return mInstance;
    }

    public void setContext(Context context) {
        Log.d(TAG, "context: " + context.toString());
        mContext = context;

        // mContext.
    }

    // 申请所有文件访问权限
    public void requestExternalStoragePermission() {
        // 判断是否有管理外部存储的权限
        if (!Environment.isExternalStorageManager()) {
            Intent intent = new Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION);
            // intent.setData(Uri.parse("package:" + getPackageName()));
            mContext.startActivity(intent);
        }
    }

    // 定义一个静态方法，返回Environment.isExternalStorageManager()的值
    public void checkStoragePermission() {
        // 跳转到设置界面引导用户打开
        Intent intent = new Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION);
        mContext.startActivity(intent);
    }

    // 获取USB信息
    public String getUSBInfo() {
        UsbManager usbManager = (UsbManager) mContext.getSystemService(Context.USB_SERVICE);
        HashMap<String, UsbDevice> deviceList = usbManager.getDeviceList();
        Iterator<UsbDevice> deviceIterator = deviceList.values().iterator();
        List<UsbDevice> usbDevices = new ArrayList<>();
        StringBuffer stringBuffer = new StringBuffer();
        while (deviceIterator.hasNext()) {
            UsbDevice device = deviceIterator.next();
            usbDevices.add(device);
            stringBuffer.append("设备名:" + device.getDeviceName());
            stringBuffer.append(" \n");
            stringBuffer.append("制造商:" + device.getManufacturerName());
            stringBuffer.append(" \n");
            stringBuffer.append("产品名:" + device.getProductName());
            stringBuffer.append(" \n");
            stringBuffer.append("版本号:" + device.getVersion());
            stringBuffer.append(" \n");
            stringBuffer.append(" \n");
        }
        return stringBuffer.toString();
    }

    // 获取卷信息
    public String getVolumeInfo() {
        StorageManager storageManager = (StorageManager) mContext.getSystemService(Context.STORAGE_SERVICE);

        StringBuffer stringBuffer = new StringBuffer();
        try {
            List<StorageVolume> storageVolumes = storageManager.getStorageVolumes();
            stringBuffer.append("卷数量:" + storageVolumes.size());
            stringBuffer.append(" \n");
            for (StorageVolume storageVolume : storageVolumes) {
                stringBuffer.append("卷描述:" + storageVolume.getDescription(mContext));
                stringBuffer.append(" \n");
                stringBuffer.append("卷UUID:" + storageVolume.getUuid());
                stringBuffer.append(" \n");
                stringBuffer.append("卷状态:" + storageVolume.getState());
                stringBuffer.append(" \n");
                stringBuffer.append("卷文件路径:" + storageVolume.getDirectory().getAbsolutePath());
                stringBuffer.append(" \n");
                String isRemovableString = storageVolume.isRemovable() ? "可移动" : "不可移动";
                stringBuffer.append("卷Removable:" + isRemovableString);
                stringBuffer.append(" \n");
                stringBuffer.append(" \n");
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        return stringBuffer.toString();
    }

    public String getUSBPath()
    {
        StorageManager storageManager = (StorageManager) mContext.getSystemService(Context.STORAGE_SERVICE);

        StringBuffer sBuffer = new StringBuffer("");
        try {
            List<StorageVolume> storageVolumes = storageManager.getStorageVolumes();
            for (StorageVolume storageVolume : storageVolumes) 
            {
                if (storageVolume.isRemovable())
                {
                    String usbPath = storageVolume.getDirectory().getAbsolutePath();
                    sBuffer.append(usbPath);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        return sBuffer.toString();
    }

    // 测试返回String[]类型
    public String[] getStringArray() {
        // 创建一个String数组
        String[] array = new String[] { "One", "Two", "Three" };
        return array;
    }

    // 测试返回String类型
    public String getStringFromJava() {
        StringBuffer stringBuffer = new StringBuffer("Hello, Java World!");
        return stringBuffer.toString();
    }

    // 全屏
    public void fullScreenStickyImmersive() {
        Activity activity = (Activity) mContext;
        View decorView = activity.getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                        | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_FULLSCREEN);
    }

    // 重启
    public void doreboot() {
        try {
            Runtime.getRuntime().exec(new String[] { "reboot" });
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    // 关机
    public void doshutdown() {
        try {
            Runtime.getRuntime().exec(new String[] { "reboot", "-p" });
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    // 获取su权限
    public void getsu() {
        Process process = null;
        DataOutputStream os = null;

        try {
            process = Runtime.getRuntime().exec("su");
            os = new DataOutputStream(process.getOutputStream());
            os.writeBytes("service call statusbar 2\n");
            os.flush();
            os.writeBytes("exit\n");
            os.flush();
            process.waitFor();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                if (os != null) {
                    os.close();
                }
                if (process != null) {
                    process.destroy();
                }
            } catch (Exception ignored) {
            }
        }
    }

    // 运行app
    public void runApp(String packetname, String activitypage) {
        Intent intent = new Intent(Intent.ACTION_MAIN);
        intent.addCategory(Intent.CATEGORY_LAUNCHER);
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        ComponentName cn = new ComponentName(packetname, activitypage);
        intent.setComponent(cn);

        Activity activity = (Activity) mContext;
        activity.startActivity(intent);
    }

}

/*****************************
 * DocumentFile********************************************
 */
