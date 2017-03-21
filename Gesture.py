import bge
E = bge.events
L = bge.logic
scene=bge.logic.getCurrentScene()
cont=bge.logic.getCurrentController()
self=cont.owner
keyboard=cont.sensors['Keyboard']

L1=cont.actuators['L1stFinger']
L2=cont.actuators['L2ndFinger']
L3=cont.actuators['L3rdFinger']
L4=cont.actuators['L4thFinger']
LT=cont.actuators['LThumb']

R1=cont.actuators['R1stFinger']
R2=cont.actuators['R2ndFinger']
R3=cont.actuators['R3rdFinger']
R4=cont.actuators['R4thFinger']
RT=cont.actuators['RThumb']

fingers = {'R':{'1':R1,'2':R2,'3':R3,'4':R4,'T':RT},
           'L':{'1':L1,'2':L2,'3':L3,'4':L4,'T':LT}}

fingerStatus= {'R':{'1':"Opening",'2':"Opening",'3':"Opening",'4':"Opening",'T':"Opening"},
               'L':{'1':"Opening",'2':"Opening",'3':"Opening",'4':"Opening",'T':"Opening"}}

stati = {"Opening":"Open","Closing":"Close"}

buttonEquil={E.LEFTSHIFTKEY:'L4',E.AKEY:'L3',E.SKEY:'L2',E.DKEY:'L1',E.SPACEKEY:'LT',
             E.RIGHTARROWKEY:'RT',E.PAD4:'R1',E.PAD5:'R2',E.PAD6:'R3',E.PADENTER:'R4'}

logicEquil = {L.KX_INPUT_JUST_ACTIVATED:"Closing",L.KX_INPUT_JUST_RELEASED:"Opening"}

def Main():
    for side in fingers:
        for digit in fingers[side]:
            cont.activate(fingers[side][digit])
            print(fingers[side][digit].action)
    GetInput()
    Update()
    print("==============")

def GetInput():
    for key,status in keyboard.events:
        if key in buttonEquil and status in logicEquil:
            finger_index = buttonEquil[key]
            currentStatus = fingerStatus[finger_index[0]][finger_index[1]]
            print(finger_index, currentStatus)
            if not currentStatus in stati:
                fingerStatus[finger_index[0]][finger_index[1]] = logicEquil[status]

def Update():
    for side in fingerStatus:
        for digit in fingerStatus[side]:
            fingers[side][digit].action = side+digit+'Open'
            fingers[side][digit].frameEnd = 5
