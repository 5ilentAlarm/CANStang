# CAN Peripheral on the STM32G431
The G431RB ha a CANFD peripheral, so its compliant with all the CAN standards that are used at the moment. Just make sure that the peripheral is configured in normal mode, to allow for full compatibility. 

## Peripheral description 
Found in [Datsheet](/Users/sebastian/Development/Embedded/CANStang/Docs/STM32G4_Series_Datasheet.pdf), page 1950.

## Pins
The CANRX and CANTX pins are:
- PA11 = FDCAN_RX
- PA12 = FD_CANTX

The pin names on the nucleo have the same designation. 

## Notes about CAN in the S550
### CAN configuration
The CAN bus on my 2015 Mustang GT runs at 500 Kbps, uses the 2.0B CAN standard, with an 11-bit identifier. 

### Filtering ID's
The MCU has hardware meant for filtering incoming CAN messages, this *should* be implemented, has having the filtering done in software seems too easy...

The ID's that are important are found [here](/Users/sebastian/Development/Embedded/CANStang/Docs/CAN_IDs.md)
but for quick reference: 
- 0x109 for RPMs
- There are too many speed related ID's to list...





