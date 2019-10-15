Connecting to [BlueCrystal](https://www.acrc.bris.ac.uk/)
=========================

use [BlueCrystal Phase 4](https://www.acrc.bris.ac.uk/acrc/phase4.htm) (BCp4).

```
$ ssh <username>@bc4login.acrc.bris.ac.uk
```

### Running graphical programs

If you want to run a program that has a GUI, you need to enable X forwarding:

```
$ ssh -X <username>@bc4login.acrc.bris.ac.uk
```

You can test it works by running a program with a graphical interface, for example:

```bash
$ xeyes
```

You should see a text editor window (running on BlueCrystal) on your screen.
If you get a `Can't open display error`, then you have not used `-X`.
You will need to log out and log back in using the forwarding option.
If you get a warning message about failing to enable _untrusted_ SSH X forwarding when you log in, or if you _can_ start GUI programs but parts of the interface are missing, try logging out and using the trusted forwarding option instead:

```
$ ssh -Y <username>@bc4login.acrc.bris.ac.uk
```

You should now be able to connect to BlueCrystal easily from your local machine:

```bash
$ ssh bcp4
[ab12345@bc4login2 ~]$
```

### Connecting from outside the University

If you want to connect to BlueCrystal from outside the University, you first need to first connect to the University's network.

Your first option is to use [the University's VPN](https://www.bris.ac.uk/it-services/advice/homeusers/uobonly/uobvpn/).
Once you set this up and connect to it, your traffic will be tunneled to a server on the University's network.
From there you can access BlueCrystal (and any other University resources) as if you were connected to eduroam.
**WARNING**: While connected to the VPN, _all your traffic will go via the University_, so make sure to disable it when you don't need it.

The other option is to connect to seis, a server in the CS Department that exists specifically to enable proxy access to other systems.
This server will not offer you an environment where you can work, but from it you can connect to BlueCrystal.
To connect to seis, use your UoB username and password:

```
$ ssh <username>@seis.bris.ac.uk
```

If you have [set up access using an SSH key](#passwordless-ssh-access), then you can take a step further and automate connecting to seis and from there to BlueCrystal.
First, set up your SSH key on snowy following the same procedure you used for BlueCrystal and confirm it works, i.e. that you can connect without typing your password.
Then, add the following `ProxyCommand` line to your SSH configuration for BlueCrystal (the example assumes that you have used the alias `seis`):

```
ProxyCommand ssh -q -W %h:%p seis
```

You can find more details about SSH proxy jumps [on WikiBooks](https://en.wikibooks.org/wiki/OpenSSH%2FCookbook%2FProxies_and_Jump_Hosts#Passing_Through_One_or_More_Gateways_Using_ProxyJump).
In particular, if you have a recent version of OpenSSH, then you can use the `ProxyJump` directive.

If you have followed all the steps so far, your SSH configuration file should contain the following (but using your own username):

```
IdentitiesOnly yes

Host seis
    HostName seis.cs.bris.ac.uk
    User ab12345
    IdentityFile ~/.ssh/uob

Host bcp4
    HostName bc4login.acrc.bris.ac.uk
    User ab12345
    IdentityFile ~/.ssh/uob
    ProxyCommand ssh -q -W %h:%p seis
```

If you want to [use GUI applications](#running-graphical-programs) when connecting with a proxy jump, you will need to use `-X` (or `-Y`) for _all_ the proxy connections.
If you use the `ProxyCommand` configuration, just add the flag to the `ssh` command line.

## Transferring files

The easiest way to transfer files to BlueCrystal is through SFTP, for which we will use the `scp` command.
`scp` works just like `cp`, in that its first argument is the source, the second one is the destination, and it will _not_ copy directories recursively unless given the `-r` flag.
In addition, you can specify a remote host for either the source or the destination (or even both, but there are [some subtleties](https://superuser.com/a/686527)) using the colon syntax:

```bash
$ scp bcp4:hello.c test.c #1
$ scp hello.c bcp4:test.c #2
```

In the example above, the first command _downloads_ `hello.c` from BlueCrystal into a local file called `test.c`.
The second command does the reverse process, _uploading_ the local `hello.c` file to a remote file called `test.c`.
Both commands assume you have followed the [SSH key setup above](#passwordless-ssh-access), which we recommend; if you haven't, then you will need to use the full `username@host` syntax and enter your password:

```bash
$ scp hello.c ab12345@bc4login.acrc.bris.ac.uk:test.c
```

If you want to use a GUI for your file transfers, you can use:

- Your native file explorer on Linux
- [WinSCP](https://winscp.net/eng/index.php) on Windows
- [Cyberduck](https://cyberduck.io/) on Mac

## Editing files remotely

Depending on your editor of choice, you will either be editing files on your own machine and transferring them using, for example, SSHFS, or editing directly on BlueCrystal.

If you want to edit files locally and transfer, some options are discussed in [_Prerequisites_](0_Prerequisites.md#required-and-useful-tools).

If your editor supports editing files remotely, then you can use your _local editor_ to open _files on BlueCrystal_).
For example, you can achieve this using [a VS Code plugin](https://marketplace.visualstudio.com/items?itemName=rafaelmaiolla.remote-vscode) or [the `scp://` scheme in Vim](http://vim.wikia.com/wiki/Editing_remote_files_via_scp_in_vim).

Finally, you can use a terminal editor on BlueCrystal.
Emacs, Vim, and nano are available, among others.
If you decide to use Vim and you haven't used it before, you can greatly improve the default configuration; see [amix's basic vimrc](https://github.com/amix/vimrc/blob/master/vimrcs/basic.vim) for a good starting point or [Will Price's vim configuration](https://github.com/willprice/dotfiles/tree/master/vim/.vim) for a more advanced setup.
Emacs has more sensible defaults, so you may find that it needs less tweaking.
Using nano on anything else than a temporary basis or in emergencies is controversial...

## Further resources

A variety of guides and cheat sheets on using BlueCrystal are available on the ACRC website: <https://www.acrc.bris.ac.uk/acrc/resources.htm>.
