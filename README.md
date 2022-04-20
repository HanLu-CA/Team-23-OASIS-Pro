# COMP 3004 FINAL PROJECT TEAM 23

##### MOJEED GEORGE

- [x] UI
- [x] USE CASE MODEL
- [x] USE CASE DIAGRAM
- [x] RECORD CLASS IMPLEMENTATION
- [x] SESSION CLASS IMPLEMENTATION

##### HAN LU

- [x] UI
- [x] UML CLASS DIAGRAM
- [x] CODE DESIGN
- [x] BUTTONS FEATURE
- [x] PROGRAM LOGIC

##### JOHN NGUYEN

- [x] UI
- [x] BUTTONS FEATURE
- [x] PROGRAM LOGIC
- [x] SESSION CLASS IMPLEMENTATION
- [x] RECORD CLASS IMPLEMENTATION

##### XIULING PAN

- [x] UI
- [x] SEQUENCE DIAGRAM
- [x] TRACEABILITY MATRIX
- [x] BUTTONS FEATURE
- [x] PROGRAM LOGIC

### FILE ORGANIZATION

1. Design documentation
2. Res
   3.src
   4.ui

In the design documentation folder, it includes the all the sequence diagrams, traceability matrix, oasis pro use case, oasis pro use case diagram and the uml class diagram

In the res folder, it has all the buttons, icons and overlay we used for the ui design

In the src folder, it includes all the source codes, which are main.cpp, mainwindow.cpp, mainwindow.h, record.cpp, record.h, session.cpp and session.h

In the ui folder, it contains the mainwindow.ui 

#### INSTRUCTION

- Press the Power Button to turn on the device (3s delay)
- Press connection buttons to connect CES input, left/right earclip (these has to be turned on in order to run a session)
- Select a session type and length, then press Select Button to start the session
- To create a User designed session, select User desgined, press Select Button, then choose a session type and length, then press Select Button to save it
- To run a User designed session, select the User designed session in the list, then long press the Select Button
- There is a speed mode that speed up the timer by 10 times, it is automatically turned on
- There is a recording mode that record any session ran by user, it is automatically turned on
- There is a battery charge button that charge the battery back to 100%
- There is a Pause/Resume button that pause a running session or resume a pause session.
- Press the INT up/down button to increase/decrease intensity (this can also be done during a running session)
- Select the connectivity from the drop-down list to manually adjust the connectivity

#### TESTED SCENARIOS

The scenarios which worked are

- Turning on the device with a long hold until the led lights come on
- Turning off the device with a long hold until the led lights go off
- Selecting a session
- Ending a session
- Saving user designated session
- Connecting the earclips and disconnecting them
- Connection test
- We are able to adjust the intensity
- We are able to save preferences
- Pausing and resuming the session and so on.
