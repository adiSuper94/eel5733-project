# Deadlocks

## Deadlock 1
- In `MODE1` when the `e2_open` function is called we call down on `sem2`, which is upped in `e2_release`.
- So if a user space process opens the device file twices and then closes both of them, we will be deadlocked when teh 2nd open is called.
- Test code in `deadlock1.c`

## Deadlock 2
- The device is in `MODE1`, we have a one user spce process with 2 threads.
- One thread tries to open the device change the mode to `MODE2` and closes the device.
- Second thread just opens and closes the device.
- In this case, it is possible that the 2nd thread might have to wait when it opens the device, as the device might still be in `MODE1`.
- If 2nd thread is waiting before 1st thread changes the mode, the `count1` will be `2`.
- When first thread calls ioctl for mode switch, it might be deadlocked as it is waiting for device count to be `1`.
- Test code in `deadlock2.c`

## Deadlock 3
- This is very similar to Deadlock 2, the main difference is that before spanning threads we ensure that thread is in `MODE2`.
- We have a thread function that opens the device and chnages the mode to `MODE1`.
- Since 2 thread have opened the device `count2` will be `2` and both the threads will be deadlocked waiting for it to go down to 1.
- More specifically, the deadlock will happen in file `assignment6.c`, function `ec_ioctl`, in `MODE1` case's `wait_event_interuptible`. 
- Test code is in `deadlock3.c`
