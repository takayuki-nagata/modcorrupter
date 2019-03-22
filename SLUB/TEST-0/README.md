# Test Cases and Steps
## TEST-0_CASE-1: after creating slab cache

0. create corrupter_slab_cache with a constructor which fills all objects with 0x88.

## TEST-0_CASE-2: after allocating 1st object

1. alloc obj0.

## TEST-0_CASE-3: after allocating 2nd object

2. alloc obj1.

## TEST-0_CASE-4: after updating contents of the 1st object

3. fill contents of obj0 with 0x11.

## TEST-0_CASE-5: after freeing the 1st object

4. free obj0.

## TEST-0_CASE-6: after freeing the 2nd object

5. free obj1.

## TEST-0_CASE-7: after destroying slab cache

6. destroy corrupter_slab_cache.

# Test Results
## Red Hat Enterprise Linux 7
### TEST-0_CASE-1: after creating slab cache
### TEST-0_CASE-2: after allocating 1st object
### TEST-0_CASE-3: after allocating 2nd object
### TEST-0_CASE-4: after updating contents of the 1st object
### TEST-0_CASE-5: after freeing the 1st object
### TEST-0_CASE-6: after freeing the 2nd object
### TEST-0_CASE-7: after destroying slab cache
