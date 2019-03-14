# A. corrupt with memory overrun

~~~
memory overrun
+-------------->
+----+---+----+---+----+---+-
|obj0|fp0|obj1|fp1|obj2|fp2|...
+----+---+----+---+----+---+-
~~~

## TEST A-1. corrupt fp of free object with memory overrun

1. alloc obj0.
2. corrupt fp1 with memory overrun using obj0.

Before:
~~~
freelist -> obj0 -> obj1 -> obj2 -> ...
~~~

~~~
 obj0|fp0  <- freelist
 obj1|fp1
 obj2|fp2
 ...
~~~

After:
~~~
freelist -> obj1 -> ??? (obj2) -> ...
~~~

~~~
[obj0|fp0]
 obj1|fp1  <- freelist
 obj2|fp2
 ...
~~~

### TEST A-2. corrupt freelist with allocating new object

3. alloc obj1

After:
~~~
freelist -> ??? (obj2) -> ...
~~~

~~~
[obj0|fp0]
[obj1|fp1]
 obj2|fp2   ??? <- freelist
 ...
~~~

#### TEST A-3a. use corrupted freelist with allocating new object

4. alloc obj2

#### TEST A-3b. use corrupted freelist with freeing allocated object

4. free obj1

#### TEST A-3c. free all allocated objects, and unload module

4. free obj1
5. free obj0
6. unload module

# B. corrupt with memory underrun

~~~
     underrun
       <-+
+----+---+----+---+----+---+-
|obj0|fp0|obj1|fp1|obj2|fp2|...
+----+---+----+---+----+---+-
~~~

## TEST B-1. corrupt fp of free object with memory underrun

1. alloc obj0
2. alloc obj1
3. free obj0
4. corrupt fp0 with memory underrun using obj1

Before:
~~~
freelist -> obj0 -> obj1 -> obj2 -> ...
~~~

~~~
 obj0|fp0  <- freelist
 obj1|fp1
 obj2|fp2
 ...
~~~

After:
~~~
freelist -> obj0 -> ??? (obj2) -> ...
~~~

~~~
 obj0|fp0   <- freelist
[obj1|fp1]
 obj2|fp2
 ...
~~~

### TEST B-2. corrupt freelist with allocating new object

5. alloc obj0

After:
~~~
freelist -> ??? (obj2) -> ...
~~~

~~~
[obj0|fp0]
[obj1|fp1]
 obj2|fp2  ??? <- freelist
 ...
~~~

#### TEST B-3a. use corrupted freelist with allocating new object

6. alloc obj2

#### TEST B-3b. use corrupted freelist with freeing existing object

6. free obj1

#### TEST B-3c. free all allocated objects, and unload module

6. free obj1
7. free obj0
8. rmmod
