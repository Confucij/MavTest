#!/usr/bin/python3

from pymavlink.dialects.v20 import common as mavlink2
from pymavlink import mavutil
from time import time


TERMINAL = "/dev/ttyUSB0"
BAUD = 115200
PING_TIME = 5
SYSTEM_ID = 10
COMPONENT_ID = mavlink2.MAV_COMP_ID_USER1

connection = mavutil.mavlink_connection(TERMINAL, baud=BAUD, dialect="common")
connection.target_system = SYSTEM_ID
connection.target_component = COMPONENT_ID
connection.wait_heartbeat() # FIXME: For some reason it returns 0:0 as system and component id
print(f"Connecting to system:component ({connection.target_system}:{connection.target_component})")

next_time = time() + PING_TIME
heartbeat_times = 0
while True: 
    msg = connection.recv_match(type='HEARTBEAT', blocking=True, timeout=next_time - time())
    if msg is not None:
        heartbeat_times += 1
        print(msg)
    if time() >= next_time:
        if heartbeat_times == 0:
            print("No heartbeat from sysem in 5 seconds. Disconnecting")
            break
        heartbeat_times = 0
        print("Sending MAV_CMD_USER_1 to target")
        connection.mav.command_long_send(
            connection.target_system,
            connection.target_component,
            mavlink2.MAV_CMD_USER_1,
            0,0,0,0,0,0,0,0
        )
        next_time = time() + PING_TIME
    


    




