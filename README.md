# eWinch LoRa Controller

The scope of this project is to replace the Futaba controller from [Miami Paragliding's eWinch](http://miamiparagliding.com/ewinch/) with something more manageable & stowable for self towing. This can be extended to be used for other uses however where you may want to control an RC device over LoRa.

## <b>Disclaimer:</b> This is not tested/a work in progress and should not be used without fully understanding the limitations/technology of the device and technology. Towing Paragliders or Hang Gliders may result in life-changing injury or death. Use this controller at your own risk.


![alt text](https://i.imgur.com/fqFhmuR.jpg)

# FAQ
1. <b>How is the Range? </b>

I still have no idea how the range is. In theory LoRa is reliable up to several kilometers. Preliminary testing shows promising results with line of sight and it should be good to go for the winch.

2. <b> Build Me One?</b> 

Sorry... not right now. I open sourced this so you can build it yourself! Or find someone to do it for you. I'm not willing to currently accept the liability that goes with selling these and they're kind of a pain in the butt to solder together.

3. <b>Do you have a roadmap?</b> 

Right now everything does fit in the nunchuck, but not super well and It's probably straining the solderjoints a bit. I would like to 3d print an enclosure similar to [this](https://www.thingiverse.com/thing:3597240) for the nunchuck where everything could be firmly attached or forgo the nunchuck for something like [this](https://www.thingiverse.com/thing:4192030) I just couldn't be bothered to iterate on the design and like the nunchuck a lot.

I would also like to make a better LED error cheat sheet for everything for troubleshooting.

4. <b> Can I Contribute? </b>

Yes! Feel free to submit a pull request. I would also love it if someone wanted to tackle a better controller housing.

# Build Guide

This guide assumes you have some soldering ability and a basic-ish understanding of microcontrollers.

## Consumables Needed
#### <b>Item quantity refers to individual items needed. Some of the links are multi-packs of the item. It's always nice to have extra in case you mess up while soldering.</b> At the time of writing this, everything comes out to <150$

| Items      | Link | Quantity|
| ----------- | ----------- | ----|
| Wii Nunchuck      | https://www.adafruit.com/product/342 | 1 |
| Adafruit M0 LoRa Feather   | https://www.adafruit.com/product/3178        | 2 |
| 400 mA Lipo   | https://www.adafruit.com/product/3898        | 1+ |
| 6 in. U.FL to SMA Female Pigtail Adapter   | https://www.amazon.com/gp/product/B01HXU1PKS        | 2+ |
| High Knob On/Off Switch   | https://www.amazon.com/gp/product/B07BD1SPYG        | 1 |
| RGB LED   | https://www.amazon.com/gp/product/B077X95LRZ        | 1 |
| UFL SMT Antenna Element Base  | https://www.amazon.com/IPX-UFL-Connector-Straight-Goldplated-Shipping/dp/B071JNGHWQ | 2+
| Antenna For Controller   | https://www.amazon.com/gp/product/B00N4Y2C4G        | 1 |
| Antenna For Receiver   | https://www.amazon.com/gp/product/B091PRHPTJ        | 1 |
| 25 Ft. 5 pin DMX   | https://www.amazon.com/gp/product/B00CTSNQPC        | 1 |
| Project Box (I bought at Lowes But this one looks okay) | https://www.amazon.com/LeMotech-Plastic-Electrical-Project-Junction/dp/B07W6SJGJM | 1 |
| Various Heatshrink | https://www.amazon.com/625pcs-Shrink-Tubing-Tubes-Ratio/dp/B07QM8249H | 1 Package |
| Various 24AWG Braided Wire | https://www.amazon.com/gp/product/B089D13Y1N | 1 Package |

## Assembling the Receiver
1. Cut the DMX cable about 1.5' from the male connector so the female pin is on the long piece. Strip the sleeve, try not to destroy the metal sheilding as this is your ground.
2. Go ahead and peel back the sheilding wire and twist it and wrap in heat shrink wrap and put a piece of shrink wrap on the wire if you wish.
3. Drill a hole in your project box big enough for the cable and run it through. Drill a 1/4" hole for your antenna extender and go ahead and tighten it through.
4. Solder the U.Fl mount onto your feather following [this guide](https://learn.adafruit.com/adafruit-feather-m0-radio-with-lora-radio-module/antenna-options)
5. Solder the wires from the female connector to the feather as follows. You may want to solder them to solid core jumpers first, or use a crimper. 
    1. Heat Shrinked Sheathing -> Gnd
    2. Red -> USB (Power: Use usb as it's regulated power. Do not go to 3.3 or JST)
    3. Black -> 5 (Throttle Channel)
    4. White -> 6 (Aux Switch Channel)
    5. Green -> Unused

6. Glue the cable in with hot glue or zip tie it so it stays secure.
7. Attach the antenna extender to the feather surface mount and screw the antenna on.

## Assemble the Controller
Imgur Album - https://imgur.com/a/Z0dtbzv
1. Unscrew the nunchuck. It has 2 y-shaped screws, but you can also use a flat head.
2. Cut the wire from the outside of the nunchuck and pull out the circuit board and buttons. Try not to lose anything yet.
3. Gut the handle of the nunchuck out totally with an exacto knife, chisel, whatever works. You want the internal rafting to be totally removed up to the divider that aligns with the top screw and to cut a notch out of the divider. See album.
4. drill a 1/4" hole where the bottom screw was for the antenna to go. Drill a hole for the LED and cut a rectangle for the switch with a drill and exacto knife (or a sacrificial soldering iron tip) on the side of the controller a bit below the edge.
5. Cut the circuit board so it doesn't overhang but still has the joystick attached.
Now for the hard part...
6. Cut the connection from the double button circuit board. Desolder the wire and resolder with ~6" longer wire with the buttons facing away Black, Red, White. White is Lower Button and Red is Upper and Black is GND.
7. Desolder the joystick from the circuit board and take note of which one is the vertical potentiometer. Drill out the solder holes a big enough diameter to fit the 24 gauge wire through and solder 6" of the wire directly to the potentiometer pins. Left to right it's Power - Red, Signal - Green, Ground - Black. Put heat shrink wrap over the joints to clean it up and thread back through the circuit board
8. Solder the 4 stand pins back to the circuit board so the joystick will stay in place in the controller.
9. Solder wire up to the LED, a middle and side pin of the switch and cover with heat wrap.
10. Solder everything together according to the wiring diagram. It will be a tight squeeze, but should all fit.
### Wiring Diagram
![Wiring Diagram](https://i.imgur.com/3M12MWh.png)
### Pinout
![Pinout](https://cdn-learn.adafruit.com/assets/assets/000/046/254/original/feather_Feather_M0_LoRa_v1.2-1.png)


## Upload Everything
1. Install PIO to your computer & VS Code.
2. Update the platform.ini folder to match the respective ports for your boards.
3. Upload the Transmitter code to the transmitter board
4. Upload the Receiver board to the receiver.

## Testing
To test, plug in the receiver to the winch and set it up on a stand or the back of your car. <b> BE READY FOR YOUR WINCH TO SPIN!</b> Turn the winch on. If it starts spinning, turn it off immediately. You will need to adjust the mapping `int val = map(input->getPotValueFlipped(), 0, 1000, 1000, 2000);` in mainReceiver until it sends a neutral value for your winch. Mine is right between 1000 and 2000 at 1500 so 500 on the nunchuck maps to it. Try tweaking those values, reuploading and trying again.

Turn on your controller and try to spin the winch.

## Using the controller

The idea is that the controller won't send a signal to the motor unless the top button is being held down as a sort of deadman switch. The lower button is the aux switch so holding it down spins the winch without autostop.

Forward on the joystick is throttle and back is the "crawl" mode, same as the futaba controller. Remember to hold down the top button on the nunchuck to do both. Otherwise, it will not spin.

### LED Errors
TODO: Update with more specific errors. 
Generally, green is good to go, red is bad. Don't try to use it if the controller is blinking red. If you are unsure, try using the serial monitor.