# Usage

~~~
# insmod TEST-$TESTNUM/corrupter_TEST-$TESTNUM_CASEA-$CASENUM.ko
~~~

Use `pad` to fill objects.

~~~
# insmod TEST-$TESTNUM/corrupter_TEST-$TESTNUM_CASEA-$CASENUM.ko pad=0x88
~~~

# Test Cases
## TEST-0: no corruption
Refer to [README for TEST-0](TEST-0/README.md)

## TEST-A: corruption with memory overrun
Refer to [README for TEST-A](TEST-A/README.md)

## TEST-B: corruption with memory underrun
Refer to [README for TEST-B](TEST-B/README.md)

# Reference
<https://events.static.linuxfound.org/sites/events/files/slides/slaballocators.pdf>
