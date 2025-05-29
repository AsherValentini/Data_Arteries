#Overview

Type				                Mechanism		          Memory			          Concurrency			          Blocking		Used for
Atomic Queue (SPSC)		      Lock-free		          Dynamic			          1P + 1C				            N			      Real-time, fastest path
Blocking Queue (SPSC)		    Mutex + CondVar		    Dynamic or pooled	    1P + 1C				            Y			      General use, safe delivery
Atomic Ring Buffer (SPSC)	  Lock-free		          Fixed/static		      1P + 1C				            N			      High-frequency streams
Blocking Ring Buffer		    Mutex + CondVar		    Fixed/static		      1P + N C or M P + N C		  Y			      Real-time with correctness
