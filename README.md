## Start
1. [CREATE ACCOUNT](https://login.arduino.cc/login?state=hKFo2SBTRG5CSXJOM2VPTDdjLWxTNTg4VVJGamw5MEp0QXNXV6FupWxvZ2luo3RpZNkgUWlXR29qWGN1V1d5MFZrRm9zM2ZyMWdMM09ZVnNJeUOjY2lk2SBkdHFwT1VlMDVxckx2YkdST0lXWWkyeG80M1ZQMFVyMw&client=dtqpOUe05qrLvbGROIWYi2xo43VP0Ur3&protocol=oauth2&authorizeTimeoutInSeconds=5&redirect_uri=https%3A%2F%2Fid.arduino.cc&scope=openid+profile+email&response_type=code&response_mode=query&nonce=WHhyU01IX1B5b2FEWndYb0cya3FqZUZWSkVUaHF0dlR2OEpIVjVETjJ1Vg%3D%3D&code_challenge=1RLR5WQYHOI3w_bo5ntv-eXzjcTkUcG31N1BDX3hS3c&code_challenge_method=S256&auth0Client=eyJuYW1lIjoiYXV0aDAtc3BhLWpzIiwidmVyc2lvbiI6IjEuMTIuMSJ9#/sso/login)
2. [ARDUINO ESP32 DOCS](https://docs.arduino.cc/arduino-cloud/guides/esp32/)
3. TOP RIGHT BROWSER:
	1. Go to Arduino Cloud -> ![[Pasted image 20250307183536.png]]
4. -> DEVICES ON LEFT NAV BAR
5. -> CLICK ![[Pasted image 20250307183801.png]]
6. -> SELECT THIRD PARTY DEVICE
7. -> SETUP DEVICE MODAL
	1. ESP8266 SELECT
	2. NodeMCU 1.0 (ESP-12E Module)
	3. Continue...
8. -> GIVE YOUR DEVICE A NAME
	1. NEXT
9. *IMPORTANT*
	1. -> SAVE ID
	2. -> SAVE SECRET KEY
	3. -> (DOWNLOAD PDF)
	4. CONFIRM SAVED + CONTINUE
10. DONE
## Create a Thing
1. THING == VIRUTAL TWIN OF HARDWARE
	1. FOR SYHNCING BETWEEN CLOUD AND THE BOARD
2. THINGS (left NAVBAR)
	1. -> CLICK CREATE A THING
3. -> ASSOCIATED DEVICE TOP RIGHT
4. -> CLICK THE LINK ICON
	1. MODAL: -> SELECT THE DEVICE YOU NAMED IN PREV. STEP
## Create a Cloud Variable
1. CLICK "ADD" Middle Top
2. "Add Variable" MODAL
	1. -> ENTER A NAME
		1. This will be a BOOLEAN VARIABLE in your CODE
	2. LEAVE EVERYTHING DEFAULT*
	3. -> CLICK "Add Variable" BUTTON
## Code (Sketch)
1. `.ino` is your `Main` program file
## Install Agent
1. -> NAVIGATE BACK TO YOUR WEB BROWSER
2. -> DOWNLOAD cloud.arduino.cc/download-agent
3. -> NAVIGATE TO THE [DOCS](https://docs.arduino.cc/arduino-cloud/guides/esp32/)
4. FOLLOW COMPILE & UPLOAD STEP
## Back to Things
1. SETUP NETWORK
2. NAVIGATE TO THINGS (LEFT SIDE NAV BAR) in CLOUD INTERFACE (not dashboard?)
3. -> CLICK ON YOUR THING
4. -> RIGHT NAVBAR UNDER "NETWORK"
	1. CLICK "LINK" CONFIGURE/CHANGE ICON
5. -> HARDCODE WIFI NETWORK AND PASSWORD INTO DEVICE
## Code (Sketch)
1. -> TOP LEFT CLICK the ARROW ->
	1. "UPLOAD" BUTTON
## Physical Tofu Space
1. 
