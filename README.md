# How to patch DWM with git branch merges

## Initial configuration

    git remote add original git://git.suckless.org/dwm
    git remote add forked <your repo>

    git pull original master
    git push forked master

    cd dwm
    git config merge.ours.driver true
    make && sudo make clean install

Don't do any changes on the master branch.

Create a new branch for every patch and then merge it to the local master. The master branch commits should have only the base code, without any generated files or any modifications whatsoever. Even your config.h changes should be in a separate branch.

## Setup a new patch

    make clean && rm -f config.h
    git checkout -b pertag original/master

    wget -P patches https://dwm.suckless.org/patches/pertag/dwm-git-20120406-pertag.diff
    git apply patches/dwm-git-20120406-pertag.diff

    git add *
    git commit -m pertag
    git checkout master

### If there are errors during patch application

- Method 1: --3way

    When the patch does not apply cleanly, fall back on 3-way merge

      git apply --3way patches/dwm-git-20120406-pertag.diff

- Method 2: --reject

    Open .rej files and make the changes manually

      git apply --reject patches/dwm-git-20120406-pertag.diff
      $EDITOR file.rej
      $EDITOR file
    rm *.rej

- Method 3: the boring but safe way

    look at the error lines, patch them in a text editor, them patch the rest normally

- Other patch error information:
  - If you have whitespace errors, you can reset the branch, remove the whitespace and try again

        git reset --hard original/master
        sed -i 's/[[:space:]]*$//' patches/patch.diff
        git apply --3way patches/patch.diff

  - You can also use

        patch -p1 < patches/patch.diff

      instead of

        git apply patches/patch.diff

## Apply a single patch from a single branch (manual)

    git merge config -m config
    make && sudo make clean install

## Apply all patches in all branches (script)

    ./suckmerge

## dwm updates

    git fetch original master
    git merge original/master

Then setup the patches in all your branches, if you feel like it.

You can use ./suckrebase for updating all the branches.

## Patch updates

Checkout to the branch, then download the new patch version and apply as normal.

To push to repository, you might need --force.

## Save all branches to your repository

    git push -u forked --all

## Recompiling

    make clean && rm config.h 
    make && sudo make clean install
    
## Other

The suckdiff, suckmerge and suckrebase scripts were copied from <https://gitlab.com/uoou/dotfiles> and then changed to my liking.

You might want to enable git rerere. It It means reuse recorded resolution. It records a merge conflict that has been resolved and
reuses it again when that merge conflict happens again.

    git config rerere.enabled true
