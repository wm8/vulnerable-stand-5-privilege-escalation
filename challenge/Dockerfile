FROM debian:bookworm-slim

RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
        openssh-server \
        sudo \
        findutils \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /var/run/sshd /home/john \
    && useradd -m -d /home/john -s /bin/bash john \
    && echo 'john:pass' | chpasswd \
    && passwd -l root

COPY sshd_config /etc/ssh/sshd_config
COPY john-find-sudoers /etc/sudoers.d/john-find

RUN chmod 600 /etc/ssh/sshd_config \
    && chmod 440 /etc/sudoers.d/john-find \
    && chown root:root /etc/sudoers.d/john-find \
    && visudo -cf /etc/sudoers.d/john-find

EXPOSE 22

CMD ["/usr/sbin/sshd", "-D", "-e"]
