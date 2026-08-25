# Repeater staging
A repo dedicated to varied Repeater collections, as well as random tests and experiments.

Includes an assembly version (Linux-specific) that is called like ./runner "intention" 1000000 (amount of repetitions), which was felt by a person, so it's useful. It's quite possible energy flow is (in part) tied to CPU usage.

`NewHash.py` is faulty and will spawn endless conhost.exe processes in Windows. It will also struggle mightily with the hashing process, only doing 150,000 hashes per second in my CPU, because the method is very slow in python.

`4.0_no_optimizations.exe` is the old Intention Repeater MAX 4.0, compiled -O0, so that nothing gets optimized away or changed.
