# Repeater staging
A repo dedicated to varied Repeater collections, as well as random tests and experiments.

Includes an assembly version (Linux-specific) that is called like ./runner "intention" 1000000 (amount of repetitions), which was felt by a person, so it's useful. It's quite possible energy flow is (in part) tied to CPU usage.

`NewHash.py` is faulty and will spawn endless conhost.exe processes in Windows. It will also struggle mightily with the hashing process, only doing 150,000 hashes per second in my CPU, because the method is very slow in python. However, SCNewHash.html/2SCNewHash.html use the same method and are likewise slow, but the energy felt is very strong. To me it was almost like a thick paste.

`4.0_no_optimizations.exe` is the old Intention Repeater MAX 4.0, compiled -O0, so that nothing gets optimized away or changed.

Cuda_SC.cu hasn't really been tested in depth.

DivinationAdvancedTool3.html and IntentionColor.html are mirrors from my other repos.

Intention_Repeater_MAX.cpp is v5.28, the final version of the 5 series.

MiniRepeater.c was an experiment of putting Repeater inside a custom-tailored C VM that only takes a set amount of memory, never more or less. Unclear utility.

RepeaterLegacy.py is a translation of 5.28 into Python by AnthroHeart. I kept it here just in case, but I believe I never used it.

ultimate_radionic_system_optimized.py is not my work. I think I had put it here to preserve it.
