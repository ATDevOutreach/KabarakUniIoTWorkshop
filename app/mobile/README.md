# Mobile Application Configuration

## How to setup (Linux) 

<details> 
<summary> 
Java 8
</summary>  

> Latest Android SDK and Platform tools are still not compatible with  JDK9+ 

+ Obtain JDK8 from Oracle [downloads page](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html). Accept the lincence and select according your platforrm. 
+ Once package is downloaded run  depending on your choice
    ```bash 
    user@host:~# tar -xzf <package.tar.gz>
    ```
    Or 
    ```bash 
    user@host:~# rpm -ivh  <package.rpm>
    ```
+ For the zipped download you'll have to manually move it to a preferred directory such as `/opt/java/` or `home/user/java` or `usr/java` : create if does not exist.
    ```bash 
    user@host:~# mv <extracted-java-jdk-folder> /home/user/java 
    ```
+ Next download a compatible JRE8 [from Oracle](http://www.oracle.com/technetwork/java/javase/downloads/jre8-downloads-2133155.html) - note your JDK8 version carefully and carry out similar extraction/installation steps as JDK8 above  
    ```bash 
    user@host:~# mv <extracted-java-jre-folder> /home/user/java 
    ``` 
+ Next add Java JRE and JDK to your system path. To do this edit your `/etc/environment` file adding the absolute path to the jre and jdk executable. Then adding `JDK_HOME` variable to point to the absoulte location on the root JDK8 folder  

    ``` 
    PATH="<other-things>:/usr/java/jdk8/bin:/usr/java/jre8/bin"
    JAVA_HOME="/usr/java/jdk8/"
    ``` 
+ Reload your configs by executing the following command
    ```bash 
    source /etc/environment
    ``` 

+ Test your installation by running the following command 
    ```bash
    user@host:~# java -version
    java version "1.8.0_171"
    Java(TM) SE Runtime Environment (build 1.8.0_171-b11)
    Java HotSpot(TM) 64-Bit Server VM (build 25.171-b11, mixed mode)
    ```    
</details>  
 
 <details>
 <summary>
 Android SDK && Tools
 </summary>  

 + Create a prefered directory such `home/user/androidsdk` or otherwise 
 + Obtain Android SDK command line tools [from the official site](https://developer.android.com/studio/#downloads) - note : they're tucked below Android Studio downloads
 + Extract to obtain a `tools` folder. Move this `tools` folder to the `home/user/androidsdk/` folder created earlier.
 + Next, download and extract Android `platform-tools` [from the official site](https://developer.android.com/studio/releases/platform-tools).
 + Move the `platforms-tools` to the `home/user/androidsdk` folder.
 + You should have such a layout  
    ```bash 
    ...
    drwxr-xr-x  5 user    user    NNNN MM DD 20:08 platform-tools
    drwxr-xr-x  6 user    user    NNNN MM DD 19:03 tools
    ```
 + Next add the following to `etc/environment` to make Android SDK utilities available system-wide     
    ``` 
    PATH="<other-things>:/home/user/androidsdk/tools:/home/user/androidsdk/platform-tools"
    ANDROID_HOME="/home/user/androidsdk"
    ```
 + Run `source /etc/environment` to reload this configuration 
 + Now you need to setup some SDKs for various API levels and build  tools
 + In your `$ANDROID_HOME/tools/bin` run the following command
    ```bash 
    ./sdkmanager "platform-tools" "platforms;android-26" "tools" "ndk-bundle" "emulator" 
    ```
 + You can view a list of available packages by running `./sdkmanager --list --channel=0`

 ## Possible Fires
 + Licenses :
    ```bash
    user@host:~# yes | $ANDROID_HOME/tools/bin/sdkmanager --licenses
    ``` 
 + Missing config file
    ```bash
    user@host:~# mkdir /home/user/.android/
    user@host:~# touch /home/user/.android/repositories.cfg
    ```
 </details>

 <details> 
 <summary> 
 Gradle 
 </summary>  

 + Download and extract Gradle [from the official downloads page](https://gradle.org/releases/) into a preferred directory such as `/opt/gradle` - be sure to select *complete* 
 + In the `etc/environment` file add the following 
    ``` 
    PATH="<other-stuff>:/opt/gradle/gradle-4.8.1/bin"
    GRADLE_HOME="/opt/gradle/gradle-4.8.1" 
    ```
 + Reload your config for the Gradle installation to be available system-wide    
 </details> 
  
 
<details>
<summary>
NodeJS / Monaca && Cordova
</summary>  

 + Install the latest version of NodeJS tooling as [described here](https://nodejs.org/en/download/package-manager/#debian-and-ubuntu-based-linux-distributions)  

 + Setup Monaca and Cordova tooling as shown 
    ```bash 
    user@host:~# npm i -g monaca cordova onsenui 
    ``` 
 > That's it.
 > To make changes to this app, save your edit and run the following - replace `android` with `ios` if you have access to a MacOS host or add both. 
  ```bash 
  user@host:~# cordova platform add android 
  user@host:~# cordova build android
  ```     
</details>