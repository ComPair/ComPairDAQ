The development uses git with a series of branches designed for specific
purposes. This document walks through the various branches by following a
feature development from its early stages to release.

| Branch        | Purpose |
| ------        | ------- |
| master        | Contains the most recent approved release.                             |
| release-#.#.# | A release candidate to be tested and patched prior to full release.    |
| dev           | The current development branch with the most recent approved features. |
| feature       | A feature branch where development on a specific item is being done.   |

# Developing a Feature
Features should be developed on independent branches. This permits shorter code
review and allows one feature to be merged even while another is being revised.
To create a new feature branch within a clone of the repository use the
following commands: 
```
git fetch origin dev
git checkout -b <feature-name> origin/dev
```
The first command updates the dev branch originating on the remote fetching all the new changes that may have been merged. The second command checkouts a new branch starting off the updated dev branch. One must provide a name replacing _feature-name_ that clearly describes the feature.

Work on the feature can now be performed with any number of commits required. If the work will take an extended amount of time, requires input from other developers, or is complete and ready to be merged, then it can be pushed to the repository with the following command:
```
git push --set-upstream origin <feature-name>
```
Where the name _feature-branch_ is replaced name of the branch.

Following the completion of the development and pushing to the repository, a
merge request can be created. Select the source branch to be your feature and
the target branch to be _dev_. The new feature automatically has the continuous
integration tests performed to insure it passes and should now be reviewed to
check for any oversights as well. If all is well the feature is then merged
with the dev branch.

# Creating a New Release
When the development branch has diverged to the point that a new release is warranted the following process is followed to prepare those changes to be merged into the master.

First a new branch is created from dev called _release-#.#.#_, where _#.#.#_ is replaced the anticipated version number of the release.
```
git fetch origin dev
git checkout -b release-#.#.# origin/dev
```

Any files are updated that may contain version numbers and committed. If any other minor patching to complete the release is required it is also committed.

Following the completion of the patching, a merge request is created. The
source branch will be the release branch and the target branch will be
_master_. The updates will be reviewed and discussed with any additional
required changes committed to the release branch. 

Once the merge is approved the new release should be tagged. A new tag should be named with the version number of the release.
