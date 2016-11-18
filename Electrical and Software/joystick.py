#!/usr/bin/python3
# joystick.py
# Author: Shane Williams
# Author contact: smw0031@auburn.edu
# Last Edit: 16 November 2016 by Eric Van Horn
# Program description:
#   joystick.py is a basic program meant to interact with a basic HID joystick device. It reads the values of the
#   joystick's axes and represents them on a display screen. This program was written for use with a
#   Microsoft Sidewinder joystick (its really old, not even supported anymore, but still a basic HID). Use with other
#   joysticks may require adjusting the index value of x and y axes in the program code.
#
# As of last edit:
#   New function writeArduino() opens serial connection to Arduino, writes joystick data, and closes serial connection.
#   Both strings sent to arduino consist of integer values. One is of the axes, this needs to be divided by 1000 in the
#   arduino code. One is the button values, 0s and 1s, to control future subfunctions
#
#   Removed writeArduino function, folded into existing structure (prevents having to close arduino communication with
#   each call).
#
# Required packages:
#   pygame
#   time
#   RPi.GPIO

import pygame
import time
import serial
import tkinter
# import RPi.GPIO as GPIO                           # Uncomment when running on Raspberry Pi

# Useful links at:
# Python drawing basics
#   https://www.cs.ucsb.edu/~pconrad/cs5nm/topics/pygame/drawing/

# GPIO.setmode(GPIO.BOARD)                          # Uncomment when running on Raspberry Pi
# GPIO.setup(12, GPIO.OUT)                          # Uncomment when running on Raspberry Pi

# p = GPIO.PWM(12, 1000)                            # Channel=12, Frequency=1000 (Uncomment on Raspberry Pi)

pygame.init()                                       # Initializes pygame module
pygame.joystick.init()                              # Initializes pygame.joystick module
time.sleep(0.5)                                     # Allow some time, just in case

def readJoystick(joystick):
    """The function readJoystick() reads all the values sent from the joystick. NOTE: The Sidewinder joystick only has
    buttons and axes. Uncomment and experiment with other lines (get_hat and get_ball) for more complex joysticks."""
    axispostn=[]                                    # creates list variable for axis position values
    for i in range(joystick.get_numaxes()):         # for loop ensures all axes are evaluated
        pygame.event.pump()                         # event.pump() is a pygame function that checks for event interrupts
        axispostn.append(joystick.get_axis(i))      # appends axis(i) value to axispostn variable
        pygame.event.pump()
    buttonvals=[]                                   # creates variable for button values
    for i in range(joystick.get_numbuttons()):      # for loop ensures all buttons are read
        pygame.event.pump()
        buttonvals.append(joystick.get_button(i))   # appends button values to buttonvals variable
        pygame.event.pump()
    # hatvals = joystick.get_hat()                  # Uncomment for bigger joysticks
    # ballpostn = joystick.get_ball()               # Uncomment for bigger joysticks

    return axispostn, buttonvals                    # add variables to return for other joysticks


if pygame.joystick.get_init():                      # Run program if joystick is detected/module is initialized
    # joystick = [pygame.joystick.Joystick(x) for x in range(pygame.joystick.get_count())]
    # print(pygame.joystick.get_count())
    joystick = pygame.joystick.Joystick(0)          # 0 is supposed to be the joystick's id. change if >1 stick attached
    joystick.init()                                 # Initializes joystick declared above
    # ### The following variables are not used in this program, but I thought they would be useful things to grab anyway ### #
    eyedee = joystick.get_id()                      # Joystick ID
    nayme = joystick.get_name()                     # Joystick name
    nmbraxes = joystick.get_numaxes()               # Number of axes
    nmballs = joystick.get_numballs()               # Number of track balls
    numbuttons = joystick.get_numbuttons()          # Number of buttons
    numhats = joystick.get_numhats()                # Number of hats
    # ### ### #
    # p.start(0)                                    # Start PWM
    # ### Open window ### #
    ard = serial.Serial('/dev/ttyACM0', 9600)
    screen = pygame.display.set_mode((400,400))     # Creates a pygame screen object to write shapes to
    while True:                                       # Run loop continuously until break
        axes, buttons = readJoystick(joystick)[0], readJoystick(joystick)[1]        # axes[0] = x, axes[1] = y, axes[3] = z
        # print(axes)                               # Uncomment if useful for debugging
        time.sleep(0.13)                            # Controls refresh timing
        xps = int(axes[0]*200+200)                  # modifies x-axis return values to be centered in middle of SCREEN
        yps = int(axes[1]*200+200)                  # modifies y-axis return values to be centered in middle of SCREEN
        zps = int((-axes[2]+1.1)*20)                # negative b/c Sidewinder joystick returns -1 for lever in high position
        screen.fill((0,0,0))                        # Writes screen full of black (erases old image
        pygame.display.update()                     # Updates display
        pygame.draw.circle(screen, (0,0,255), (xps, yps), zps, 0)       # writes blue circle at (xps,yps) of radius (zps)
        pygame.display.update()                     # Updates display

        dc = int(((-axes[2]+1)*100)/2)             # Set Duty Cycle
        # p.ChangeDutyCycle(dc)                     # Change duty cycle for p
        # print('Duty Cycle ' + str(dc))
        axes, buttons = readJoystick(joystick)
        axess = [int(xi*1000) for xi in axes]           # multiplies axes' floating points to make them ints for serial transmission
        buttonString = ''.join([str(i) for i in buttons]) # convert this list of 1s and 0s to HIGHs and LOWs, then send to Arduino
        ard.write(buttonString.encode())
        # print(axes, axess)
        print(buttonString)
        # print()                                     # Prints button values (index [1]) of return from readJoystick()
        if (buttons[0] & buttons[1]  & buttons[4] & buttons[6]):  # If buttons labelled 1,2,5,and 7 are pressed...
            break                                   # Break out of while loop

    ard.close()

joystick.quit()                                     # closes joystick object
