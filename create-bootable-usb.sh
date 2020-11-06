# Use this script to create a bootable DaxOS install on USB
# <rmidhunsuresh@gmail.com>

# USAGE
# sudo ./create-bootable-usb <block_device_name>

# WARNING
# This will completely format your block device and will lead to
# permanent data loss.
# You Have Been Warned


BLOCK_DEV_NAME=$1
BLOCK_DEV_SIZE=`blockdev --getsize64 $BLOCK_DEV_NAME`
BLOCK_DEV_THRESHOLD=10737418240

echo "Block device $BLOCK_DEV_NAME has size = $BLOCK_DEV_SIZE"
if [ $BLOCK_DEV_SIZE -ge $BLOCK_DEV_THRESHOLD ]
then
    echo "Device has more than 10GB space."
    echo "Check the name of the block device again."
    exit 1
else
    # Device is smaller
    # Proceed to copy .iso into USB device
    sudo dd if=DaxOS.iso of=$BLOCK_DEV_NAME && sync
    echo "finished :)"
fi