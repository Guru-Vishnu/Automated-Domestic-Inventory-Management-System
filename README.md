# Automated Domestic Inventory Management System
Load Cell Based Device using Arduino Uno that periodically measures the weight of essential domestic consumables, thereby keeping track of their remaining quantity and inform the user if something important is about to run out.

This project was done as a part of the course ECOE23 - Internet of Things during the January 2023 session. 

This project aims to automate maintaining essential household consumables by periodically monitoring the weights of various items using load cells, estimating when the resources would be depleted based on usage, and alerting the user if the quantities of important products fall below a preset threshold value. This system can be used to prepare a shopping list of recurring consumables and enable the user to make well-informed decisions while shopping.

The system will be present in all storage locations, such as cupboards, refrigerators, and tables across the house, where different consumables would generally be placed. The respective consumables will be kept on holders or plates attached to the measuring end of the load cells.

The proof-of-concept of the proposed system uses a weighing load cell sensor capable of measuring up to 5 kg. The load cell is connected to an HX711 24-bit analog to digital converter (ADC) breakout board from which an Arduino Uno obtains the load cell readings. The weight obtained from the sensor is used to compare with the gross and net weights of the product before usage, and thus determine the quantity left.

When using the unit for the first time, the user must set the product details such as name, gross and net weights, and threshold weight value. While this can be directly programmed using the computer for the proof-of-concept, a mobile application has been developed using the MIT App Inventor platform to improve ease of access. The user input from a smartphone will be transmitted to the intelligent storage unit using Bluetooth. An HC-05 Bluetooth Module relays this information to the Arduino Uno and will be stored in its memory. 

From this point, the unit will measure the weight of its allotted product periodically, for example, everyday early in the morning when there is no human activity. The system will alert when needed or take necessary actions. The three LEDs on the unit help in state indication and provide a quick idea of how much of the item is left. The unit can be powered using a 9 V battery. However, for larger implementation, it would be better to use a power adapter to supply power to multiple units in a single location. 

While the proof-of-concept utilizes a 5 kg load cell and thus is limited to measuring only light loads such as rice, toothpaste, soaps, oils, etc., the underlying process is the same for heavy loads such as measuring LPG level in domestic gas cylinders. This method is non-invasive, i.e., it does not require any modifications to the gas fittings to install a pressure gauge in the gas line, which would be more involved in ensuring system safety, getting safety approvals and meeting standards, gaining public trust, etc. Measuring the gross weight of cylinders is comparatively simpler and safer to determine the LPG level and inform the user in advance. 

Future improvements to the system could enable it to automatically place orders of recurring consumables and get them delivered without any user intervention. The process of manually entering product details can be simplified using barcode recognition. Further, instead of having separate units for measurement, multiple products can be placed in a single unit that can detect which of the several items in it has been used, thus reducing the number of load cells required and hence the overall cost. The units can also be integrated into the cupboards and partitions, making them ‘smart’. 

For more details, please refer: https://drive.google.com/file/d/1IQRoCJJ74t1g-ydG5z807o-S6RKxtCAb/view?usp=share_link
