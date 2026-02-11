#ifndef MAINPAGE_H
#define MAINPAGE_H

// this is the settings html page
const String page PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Smart Clock</title>
  <style>
    body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background-color: #121212; color: #e0e0e0; text-align: center; padding: 20px; }
    h1 { color: #ffffff; }
    .container { max-width: 400px; margin: 0 auto; background: #1e1e1e; padding: 20px; border-radius: 10px; box-shadow: 0 4px 15px rgba(0,0,0,0.5); }
    .input-group { margin-bottom: 15px; text-align: left; }
    label { display: block; margin-bottom: 5px; font-weight: bold; color: #bbbbbb; }
    input[type="time"], input[type="number"], select { width: 100%; padding: 10px; border-radius: 5px; border: 1px solid #333; background: #2c2c2c; color: #fff; font-size: 16px; box-sizing: border-box; }
    .days { display: flex; justify-content: space-between; margin-bottom: 15px; }
    .days div { display: flex; flex-direction: column; align-items: center; }
    .days span { font-size: 12px; margin-bottom: 5px; }
    input[type="checkbox"] { transform: scale(1.5); accent-color: #007bff; }
    button { width: 100%; padding: 12px; background-color: #007bff; color: white; border: none; border-radius: 5px; font-size: 16px; cursor: pointer; transition: background 0.3s; }
    button:hover { background-color: #0056b3; }
    #stopAlarm { background-color: #dc3545; margin-top: 20px; animation: pulse 1s infinite; }
    @keyframes pulse { 0% { transform: scale(1); } 50% { transform: scale(1.05); } 100% { transform: scale(1); } }
  </style>
</head>
<body>
  <div class="container">
    <h1>Smart Clock</h1>
    %ALARM_SWITCH%
    <form action="/setAlarm" method="POST">
      
      <div class="input-group">
        <label for="alarmTime">Alarm Time</label>
        <input type="time" id="alarmTime" name="alarmTime" value="%ALARM%" required>
      </div>

      <div class="days">
        <div><span>Sun</span><input type="checkbox" name="d1" %D1%></div>
        <div><span>Mon</span><input type="checkbox" name="d2" %D2%></div>
        <div><span>Tue</span><input type="checkbox" name="d3" %D3%></div>
        <div><span>Wed</span><input type="checkbox" name="d4" %D4%></div>
        <div><span>Thu</span><input type="checkbox" name="d5" %D5%></div>
        <div><span>Fri</span><input type="checkbox" name="d6" %D6%></div>
        <div><span>Sat</span><input type="checkbox" name="d7" %D7%></div>
      </div>

      <div class="input-group">
        <label for="timezone">Timezone</label>
        <select id="timezone" name="timezone">
           <option value="" disabled selected>Loading timezones...</option>
        </select>
      </div>

      <div class="input-group">
        <label for="brightness">Brightness (1-9)</label>
        <input type="number" id="brightness" name="brightness" min="1" max="9" value="%BRIGHTNESS%">
      </div>

      <button type="submit">Save Settings</button>
    </form>
  </div>

<script>
//  POSIX Timezones List
const timezones = [
    { name: "Europe/Lisbon (Portugal)", rule: "WET0WEST,M3.5.0/1,M10.5.0" },
    { name: "Europe/London (UK)", rule: "GMT0BST,M3.5.0/1,M10.5.0" },
    { name: "Europe/Madrid (Spain)", rule: "CET-1CEST,M3.5.0,M10.5.0/3" },
    { name: "Europe/Paris (France)", rule: "CET-1CEST,M3.5.0,M10.5.0/3" },
    { name: "Europe/Berlin (Germany)", rule: "CET-1CEST,M3.5.0,M10.5.0/3" },
    { name: "Europe/Rome (Italy)", rule: "CET-1CEST,M3.5.0,M10.5.0/3" },
    { name: "Europe/Brussels (Belgium)", rule: "CET-1CEST,M3.5.0,M10.5.0/3" },
    { name: "Europe/Amsterdam (Netherlands)", rule: "CET-1CEST,M3.5.0,M10.5.0/3" },
    { name: "Europe/Athens (Greece)", rule: "EET-2EEST,M3.5.0/3,M10.5.0/4" },
    { name: "Europe/Moscow (Russia)", rule: "MSK-3" },
    { name: "America/New_York (US-East)", rule: "EST5EDT,M3.2.0,M11.1.0" },
    { name: "America/Chicago (US-Central)", rule: "CST6CDT,M3.2.0,M11.1.0" },
    { name: "America/Sao_Paulo (Brazil)", rule: "<-03>3" },
    { name: "Asia/Dubai (UAE)", rule: "<+04>-4" },
    { name: "Asia/Tokyo (Japan)", rule: "JST-9" },
    { name: "Asia/Shanghai (China)", rule: "CST-8" },
    { name: "UTC (Universal Time)", rule: "UTC0" }
];

const currentTzRule = "%CURRENT_TZ%";
const select = document.getElementById("timezone");

function initTimezone() {
    select.innerHTML = "";   
    let browserTz = "";
    try {
        browserTz = Intl.DateTimeFormat().resolvedOptions().timeZone;
        console.log("Browser Region:", browserTz);
    } catch(e) {}

    let bestMatchIndex = -1;
    let firstRuleMatchIndex = -1;

    timezones.forEach((tz, index) => {
        const option = document.createElement("option");
        option.text = tz.name;
        option.value = tz.rule;
        select.appendChild(option);

        if (currentTzRule === tz.rule) {
            if (firstRuleMatchIndex === -1) {
                firstRuleMatchIndex = index;
            }
            if (browserTz && tz.name.includes(browserTz)) {
                bestMatchIndex = index;
            }
        }
    });

    if (bestMatchIndex !== -1) {
        select.selectedIndex = bestMatchIndex;
    } else if (firstRuleMatchIndex !== -1) {
        select.selectedIndex = firstRuleMatchIndex;
    } else if (currentTzRule === "UTC0") {
        for (let i = 0; i < select.options.length; i++) {
            if (browserTz && select.options[i].text.includes(browserTz)) {
                select.selectedIndex = i;
                break;
            }
        }
    }
}

initTimezone();
</script>
</body>
</html>
)=====";

#endif