
1/ Want to put ~ files under .patches.
     .patches/orig and .patches/current

   If we are using an RCS, we don't really need orig.
DONE

2/ want to be able to push all current patches into the RCS

HALF_DONE

3/ 'Signed-off-by' should be in a file.
DONE

4/ maybe a .conf file rather than misc files.
DONE

5/ review needs to be more careful.  Don't allow
   applied patches and reviewed patches at the same time.
DONE

6/ Want to be able to find all patches that might
   be reviewable.

7/ Make it easier to unapply a patch (like discard, but uses
    last-applied and 'purge'
DONE


8/ clean out to-resolve when committing
DONE
-------------------------------------------

1-
  test for existence and move files if needed.
  have 'orig' anyway

2- generate list of patches
   back them all out
   apply them one at a time
     commit -a / rebase

3/4-
    cc Prefix tag addresses
    maintainer
    owner(.prefix)
   
    [global]
      maintainer =
      author =
      signoff =

    [Nfsd]
       tag = knfsd
       cc = addresslist

5 - file 'in-review'
6 - for / patch
