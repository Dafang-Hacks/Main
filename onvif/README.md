# Dafang Hacks Onvif Service


## Description

This service is tailored for Dafang Hacks firmware and their supported cameras.

Xiaomi-Dafang-Hacks Github: [Xiaomi-Dafang-Hacks](https://github.com/Dafang-Hacks/Xiaomi-Dafang-Hacks)

ONVIF official website: [http://www.onvif.org](http://www.onvif.org)

The web service and data binding is generated using [gSOAP](http://www.genivia.com)


## Build

For build daemon in release and debug mode:
```console
make all
```

For release:
```console
make release
```

For debugging:
```console
make debug
```


## Usage

To start the daemon manually:

```console
./onvif_srvd  --ifs wlan0 --port 8080
```

The port can be omitted which defaults to 8080


## License

[GPL](./LICENSE).
