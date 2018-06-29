let connected = false;
let received_topic = "";
var app = {

    initialize: () =>
    {
        this.bindEvents();
    },

    bindEvents: () => 
    {
        document.addEventListener('deviceready', this.onDeviceReady, false);
    },

    onDeviceReady: () => 
    {
        app.receivedEvents('deviceready');
        app.connectToServer();
        app.subscribeToHumidity();
        app.subscribeToTemperature();
    },

    subscribeToTemperature: () => {
        if (!connected) {
            ons.notification.toast("Fatal connection error", { timeout: 3500 });
        } else {
            temp_topic = "kabarak/mobile/sensors/temperature";
            window.plugins.CordovaMqTTPlugin.subscribe({
                topic: temp_topic,
                qos:1,
                success: (s) =>{
                    cordova.plugins.CordovaMqTTPlugin.listen(temp_topic, (payload, params, topic, topic_pattern) => {
                        let temp_value = JSON.stringify(payload);
                        // Alter Temperature card value
                    })
                },
                error: (e) =>
                {
                    ons.notification.toast( "Fatal error getting data: " + e, { timeout:3500 });
                }
            });
        } 
    },
    subscribeToHumidity: () => {
        if (!connected) {
            ons.notification.toast("Fatal connection error", { timeout: 3500 });
        } else {
            humidity_topic = "kabarak/mobile/sensors/humidity";
            window.plugins.CordovaMqTTPlugin.subscribe({
                topic: temp_topic,
                qos:1,
                success: (s) =>{
                    cordova.plugins.CordovaMqTTPlugin.listen(humidity_topic, (payload, params, topic, topic_pattern) => {
                        let temp_value = JSON.stringify(payload);
                        // Alter Humidty card value
                    })
                },
                error: (e) =>
                {
                    ons.notification.toast( "Fatal error getting data: " + e, { timeout:3500 });
                }
            });
        } 
    },

    connectToServer: () =>
    {
        window.plugins.CordovaMqTTPlugin.publish({
            topic:"kabarak/mobile/control/door",
            payload: "door"
        });connect({
            url: "sungura1-angani-ke-host.africastalking.com",
            port:10883,
            uname: "kabarak",
            clientId: "kabarak-app",
            success: (s) =>
            {
                connected = true;
                // Green color the bar // TODO
                ons.notification.toast("Connecteed!", { timeout: 2000});
                console.log("Connected");
            },
            error: (e) =>{
                connected = false;
                // Color the bar Red
                ons.notification.toast("Fatal error: " + e , { timeout: 3000 });
            },

            onConnectionLost: () =>
            {
                connected = false;
                // Color the bar red
                ons.notification.toast("Connection Lost", { timeout: 3000 });
            }
        });
    },

    receivedEvents : (id) => {
        document.getElementById("door-ctrl").addEventListener("touchend", (evt) => {
            if (!connected) {
                ons.notification.toast("Connection error!", { timeout: 3000 });
            } else {
                window.plugins.CordovaMqTTPlugin.publish({
                    topic:"kabarak/mobile/control/door",
                    payload: "door",
                    qos:1,
                    retain: false,
                    success: (s) => {
                        ons.notification.toast( "Command Sent", { timeout: 2000 });
                    },
                    error: (e) => {
                        ons.notification.toast( "Fatal error: " + e , { timeout: 3500 });
                    }
                });
            }
        });
        document.getElementById("window-ctrl").addEventListener("touchend", (evt) => {
            if(!connected){
                ons.notification.toast("Connection error!", { timeout: 3500 });
            } else {
                window.plugins.CordovaMqTTPlugin.publish({
                    topic: "kabarak/mobile/control/door",
                    payload: "window",
                    qos: 1,
                    retain: false,
                    success: (s) => {
                        ons.notification.toast("Command Sent", { timeout: 2000 });
                    },
                    error: (e) => {
                        ons.notification.toast("Fatal error " + e, { timeout: 3500 });
                    }
                });
            }
        });
        console.log("Event id " + id);
    }
}