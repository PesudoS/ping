# Q&A:

**Q**: vi/vim prompts "E45: readonly ...", have no permission
**A**: [vi/vim no permission](https://blog.csdn.net/benjamin_whx/article/details/43447519)
1. :%! sudo tee % /dev/null
2. input passwd
3. input 'L'
4. :q exit

**Q**: change all permissions recursively in a directory
**A**: chown -R users:username [directory]

**Q**: usual git command
**A**: [git command list](http://www.ruanyifeng.com/blog/2015/12/git-cheat-sheet.html)
```
$ git init
$ git add .
$ git commit -m "ping_v1"
$ git remote add origin git://github.com/PesudoS/ping.git
$ git pull origin master 
$ git push -u origin master
```
1. create code repository
- #create a Git code repository under the current directory
  `$ git init`

- #create a directory and initialize it as a Git code repository
  `$ git init [project-name]`

- #download a project and its whole code history
  `git clone [url]`
2. config
.gitconfig: /home/user(global configuration) and project directory(project configuration)

- #set user info for commit
  `$ git config [--global] user.name "[name]"`
  `$ git config [--global] user.email "[email address]"`

- show current git configuration
  `$ git config --list`

- edit Git configuration file
  `$ git config -e [--global]`
3. add/remove file
- #add file to staging area
  `$ git add [file1] [file2] ...`
- #add directory to staging area, including sub-directory
  `$ git add [dir]`
- 
- #add all files in the current directory to staging area
  `$ git add .`
- #request confirmation before adding every change
- #submit by several times for several changes in a file 
  `$ git add -p`
- #remove file in the working area, and put this remove into staging area
  `$ git rm [file1] [file2] ...`
- #stop tracking specific file, but this file will be saved into working area
  `$ git rm --cached [file]`
- #rename file, and put this change into staging area
  `$ git mv [file-original] [file-renamed]`
4. submit code
- #submit staging area to repository area
  `$ git commit -m [message]`
- #submit specific file in the staging area to repository area
  `$ git commit [file1] [file2] ... -m [message]`
- #submit all changes after working area committed from last time to repository area
  `$ git commit -a`
- #show diff info when committing
  `$ git commit -v`
- #use a new commit to replace last commit
- #if code has no change, rewrite commit info of last commit
  `$ git commit --amend -m [message]` 
- #redo last commit, including new changes of specific files
  `$ git commit --amend [file1] [file2] ...` 
5. branch
- #list all local branches
  `$ git branch`
- #list all remote branches
  `$ git branch -r`
- #list all local and remote branches
  `$ git branch -a`
- #build a branch and still stay at current branch
  `$ git branch [branch-name]`
- #build a branch and switch to the branch
  `$ git cheackout -b [branch]`
- #build a branch and point to specific commit
  `$ git branch [branch] [commit]`
- #build a branch and establish a tracking relationship with specific remote branch
  `$ git branch --track [branch] [remote-branch]`
- #switch to specific branch and update working area
  `$ git checkout [branch-name]`
- #switch to last branch
  `$ git checkout -`
- #establish a tracking relationship between current branch and specific remote branch
  `$ git branch --set-upstream [branch] [remote-branch]`
- #merge specific branch to current branch
  `git merge [branch]`
- #choose a commit and merge into current branch
  `git cherry-pick [commit]`
- #delete branch
  `git branch -d [branch-name]`
- #delete remote branch
  `$ git push origin --delete [branch-name]`
  `$ git branch -dr [remote/branch]`
6. tag
- #list all tag
  `$ git tag`
- #create a tag in the current commit
  `$ git tag [tag]`
- #create a tag in a specific commit
  `$ git tag [tag] [commit]`
- #del local tag
  `$ git tag -d [tag]`
- #del remote tag
  `$ git push origin :refs/tags/[tagName]`
- #show tag info
  `$ git show [tag]`
- #commit specific tag
  `$ git push [remote] [tag]`
- #commit all tags
  `$ git push [remote] --tags`
- #build a branch and point to some tag
  `$ git checkout -b [branch] [tag]`
7. show info
- #show files changed
  `$ git status`
- #show the version history of the current branch
  `$ git log`
- #show commit history and files changed every commit
  `$ git log --stat`
- #search commit history according to key
  `$ git log -S [keyword]`
- #show all changes after one commit, one line for commit
  `$ git log [tag] HEAD --pretty=format:%s`
- #show all changes after one commit, "commit description" must accord with search condition
  `$ git log [tag] HEAD --grep feature`
- #show version history of one file, including rename
  `$ git log --follow [file]`
  `$ git whatchanged [file]`
- #show every diff of a specific file 
  `$ git log -p [file]`
- #show last 5 commits
  `$ git log -5 --pretty --oneline`
- #show all users who have committed, sorted by submitted number
  `$ git shortlog -sn`
- #show the modified user and time of a specific file
  `$ git blame [file]`
- #show diff between staging area and working area
  `$ git diff`
- #show diff between staging area and last commit
  `$ git diff HEAD`
- #show diff between 2 commits
  `$ git diff [first-branch]...[second-branch]`
- #show the line number of code today
  `$ git diff --shortstat "@{0 day ago}"`
- #show metadata commited and content change
  `$ git show [commit]`
- #show files changed of one commit
  `$ git show --name-only [commit]`
- #content of some file of one submit
  `$ git show [commit]:[filename]`
- #show recent several commits of current branch
  `$ git reflog`
8. remote synchronization
- #download all changes of a remote repository
  `$ git fetch [remote]`
- #show all remote repositories
  `$ git remote -v`
- #show info of some remote repository
  `$ git remote show [remote]`
- #add a new remote repository and name
  `$ git remote add [shortname] [url]`
- #pull changes of a remote repository and merge into a local branch
  `$ git pull [remote] [branch]`
- #push a local specific branch into a remote repository
  `$ git push [remote] [branch]`
- #push current branch into a remote repository forcibly regardless of conflict
  `$ git push [remote] --force`
- #push all branches into a remote repository
  `$ git push [remote] --all`
9. Revocation
- #recover a specific file in staging area into working area
  `$ git checkout [file]`
- #recover a specific file of some commit into staging area and working area
  `$ git checkout [commit] [file]`
- #recover all files in staging area into working area
  `$ git checkout .`
- #reset a specific file in staging area, according with last commit and with working area unchanged
  `$ git reset [file]`
- #reset staging area and working area, according with last commit
  `$ git reset --hard`
- #reset the pointer of current branch to a specific commit and staging area, with working area unchanged
  `$ git reset [commit]`
- #reset HEAD of the current branch to specific commit, meanwhile staging area and working area, according with a specific commit
  `$ git reset --hard [commit]`
- #reset HEAD of the current branch to specific commit, with staging area and working area unchanged
  `$ git reset --keep [commit]`
- #create a new commit to revoke a specific commit
- #The latter changes are cancelled by the former, and apply to current branch
  `$ git revert [commit]`
- #shift out uncommited changes for a while, shift in later
  `$ git stash`
  `$ git stash pop`
10. others
- #generate a package that can be released
  `$ git archive`

