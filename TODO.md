# liblog TODO list

###### Implementation

- [ ] Use color and format manipulators from libut. (after they have been added)
- [x] Move the severity threshold from global to output based. This will make it possible to have log targets with different severity thresholds.
- [ ] Maybe use ut::observer_ptr instead of raw pointers to make intent clear.