#Описание API
После включения ESPBridge ждет от 1 до 10 секунд, затем подключается к WiFi.

###Конфигурация сериал-порта
ESPBridge принимает команды на скорости 19200, 8N1 (`Serial.begin(19200);`).

###Описание команд:
Командной последовательностью считается любая последовательность символов, заканчивающаяся символами возврата каретки (`CR`, `\r`) и перевода строки (`LF`, `\n`), которые отбрасываются при обработке команды.
Первый символ команды интерпретируется как команда, последующие символы - как данные (аргумент команды). 

| Команда | Пример | Описание |
| ------ | ------ | ------ |
| `h` | `h192.168.1.1\r\n`,<br> `hserver.skbrii.ru\r\n`| Выбрать `192.168.1.1` как сервер,<br> к которому будет производиться подключение. |
| `p` | `p80\r\n` | Выбрать порт 80 как порт сервера, к которому будет производиться подключение. Порт по умолчанию - 88. |
| `a` | `aGET /test HTTP/1.1\r\n` | Добавить к буферу строку `GET /test HTTP/1.1\r\n` (`a\r\n` добавит к буферу строку `\r\n`). | 
| `?sw` | `?sw\r\n` | Возвращает статус подключения ESPBridge к wifi-сети в виде `rFAIL_CONN2WIFI\r\n` (не подключен) или `rOK_CONN2WIFI\r\n` (подключен). |
| `?ss` | `?ss\r\n` | Возвращает статус подключения ESPBridge к серверу в виде `rOK_CONN2SERVER\r\n` (подключение установлено) или `rFAIL_NOTCONNECTEDTOSERVER\r\n` (не подключен). |
| `?bl` | `?bl\r\n` | Возвращает длину содержимого буфера в виде  `rOK_BL:25\r\n`, где 25 - длина содержимого буфера. |
| `?bc` | `?bc\r\n` | Возвращает содержимое буфера `rOK_BC:<СОДЕРЖИМОЕ БУФЕРА>\r\n`, где <СОДЕРЖИМОЕ БУФЕРА> - содержимое буфера |
| `?ip` | `?ip\r\n` | Возвращает ip ESPBridge в виде `rOK_IP:192.168.1.104\r\n`. |
| `i` | `i\r\n` | Очистить буфер. Возвращает `rOK_BUFFER_CLEANED\r\n`. |
| `c` | `c\r\n` | Поключиться к заранее выбранному серверу. |
| `C` | `C\r\n` | Поключиться к заранее выбранному серверу, отправить содержимое буфера. К ответу сервера добавляется  символ `g` в начале и `$` в конце. |
| `s` | `sGET /test HTTP 1.1\r\n` | Послать данные на подключенный сервер. К ответу сервера добавляется  символ `g` в начале и `$` в конце. |
| `S` | `S\r\n` | Послать содержимое буфера на подключенный сервер. Возвращает `rOK_BUFFER_SENDED\r\n`. |
| `d` | `d\r\n` | Отключиться от сервера. Возвращает `rOK_CONNCLOSE\r\n`. |
| `e` | `e\r\n` | Включить локальное эхо (по дефолту - включено). Возвращает `rOK_ECHO_ENABLED\r\n`. |
| `E` | `E\r\n` | Отключает локальное эхо. Возвращает `rOK_ECHO_DISABLED\r\n`. |
| `Mjson` | `Mjson\r\n` | Установка режима, при котором после соединения и отправки данных возвращается содержащийся в ответе от сервера JSON-обьект, или `rFAIL_NO_JSON:XXX`, где `XXX` - HTTP-код ответа. Возвращает `rOK_MODE_JSON\r\n`. |
| `Mhttpcode` | `Mhttpcode\r\n` | Установка режима, при котором после соединения и отправки данных возвращается HTTP-код ответа сервера. Возвращает `rOK_MODE_HTTP_CODE\r\n`. |
| `Mfull` | `Mfull\r\n` | Установка режима, при котором после соединения и отправки данных возвращается ответ сервера полностью. *Режим по умолчанию.* Возвращает `rOK_MODE_FULL\r\n`. |




###Описание ответов
На каждую командную последовательность ESPBridge отвечает.
Например, `rFAIL_SETPORT\r\n` - ответ при передаче невалидных данных командой `p` (выбор порта). 
`r` - указывает на тип сообщения (ответ)
`FAIL` - результат выполнения команды
`SETPORT` - выполнявшаяся команда (или ошибка)

Вариант ответа | Пример | Описание
----------|---------|--------
`rFAIL_UNKNOWN_COMMAND:x\r\n`|`rFAIL_UNKNOWN_COMMAND:x\r\n`| Неизвестная команда `x`.
`rOK_CONN2WIFI\r\n`|`rOK_CONN2WIFI\r\n`| Подключен к вайфай.
`rFAIL_CONN2WIFI\r\n`|`rFAIL_CONN2WIFI\r\n`| Ошибка подключения к вайфаю.
`rOK_SETHOST\r\n`|`rOK_SETHOST\r\n`| Адрес сервера сохранен.
`rFAIL_SETHOST\r\n`|`rFAIL_SETHOST\r\n`| Адрес сервера не сохранен.
`rOK_SETPORT\r\n`|`rOK_SETPORT\r\n`| Порт сохранен.
`rFAIL_SETPORT\r\n`|`rFAIL_SETPORT\r\n`| Порт не сохранен.
`rOK_CONN2SERVER\r\n`|`rOK_CONN2SERVER\r\n`| Подключен к серверу.
`rFAIL_CONN2SERVER:a\r\n`|`rFAIL_CONN2SERVER:a\r\n`| Ошибка подключения к серверу. `a` может принимать значения от -1 до -15 (с некоторыми исключениями и дополнениями), расшифровки см. [вот тут](https://github.com/esp8266/Arduino/blob/master/tools/sdk/include/espconn.h#L12).
`rFAIL_NOTCONNECTEDTOSERVER\r\n`|`rFAIL_NOTCONNECTEDTOSERVER\r\n`| Нет подключения к серверу.
`rOK_CONNCLOSE\r\n`|`rOK_CONNCLOSE\r\n`| Соединение закрыто.
`rOK_SENDDATA\r\n`|`rOK_SENDDATA\r\n`| Данные отправлены на сервер.

###Пример работы с модулем

```
E
rOK_ECHO_DISABLED;
?sw
rOK_CONN2WIFI;
E
rOK_ECHO_DISABLED;
h34.####.152
rOK_SETHOST;
p80
rOK_SETPORT;
i
rOK_BUFFER_CLEANED;
aPUT /Thingworx/Things/testThing/Properties/temperature HTTP/1.1
rOK_BUFFER_APPENDED;
aHost: 34########.152
rOK_BUFFER_APPENDED;
aappKey: cdf2################e43c4fb7
rOK_BUFFER_APPENDED;
aContent-Type: application/json
rOK_BUFFER_APPENDED;
aContent-Length: 19
rOK_BUFFER_APPENDED;
a
rOK_BUFFER_APPENDED;
a{"temperature": 17}
rOK_BUFFER_APPENDED;
C
rOK_CONN2SERVER
gHTTP/1.1 200 
X-Content-Type-Options: nosniff
X-XSS-Protection: 1; mode=block
X-FRAME-OPTIONS: SAMEORIGIN
Expires: 0
Cache-Control: no-store, no-cache
Cache-Control: post-check=0, pre-check=0
Pragma: no-cache
Content-Type: text/html;charset=UTF-8
Transfer-Encoding: chunked
Date: Wed, 01 Feb 2017 13:50:55 GMT

0
$

```
(`\r\n` опущены для простоты восприятия)