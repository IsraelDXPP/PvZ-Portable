# PvZ-Portable

<div align="center">
  <img src="icon-readme.png" alt="PvZ-Portable" width="450">
</div>

[![Ask DeepWiki](https://deepwiki.com/badge.svg)](https://deepwiki.com/wszqkzqk/PvZ-Portable)

Una versión hecha por la comunidad de **Plants vs. Zombies: Game of the Year Edition**, diseñada para funcionar en varias plataformas y ofrecer la experiencia **100% auténtica** del juego.

|            🌿 Auténtico            |                                                   🎮 Portable                                                  |       🛠️ Abierto       |
| :--------------------------------: | :------------------------------------------------------------------------------------------------------------: | :---------------------: |
| Recrea casi todo el juego original | Compatible con sistemas de 32/64 bits<br>Funciona en Linux, Windows, macOS, Android, iOS, WebAssembly, Switch… | Usa OpenGL ES 2.0 y SDL |

🌐 **¿No quieres instalarlo?** [Prueba directamente en tu navegador](https://wszqkzqk.github.io/pvz-portable-wasm/pvz-portable.html) (aún necesitas los archivos del juego).

**⚠️ Aviso:**

* Este repositorio **NO** incluye assets con copyright (imágenes, música o fuentes) de PopCap Games o Electronic Arts. Los usuarios deben aportar su propia carpeta `main.pak` y `properties/` de una copia **legalmente adquirida** de Plants vs. Zombies: GOTY Edition.
* El código es una reimplementación manual basada en documentación pública y la investigación de la comunidad (como [植物大战僵尸吧](https://tieba.baidu.com/f?ie=utf-8&kw=%E6%A4%8D%E7%89%A9%E5%A4%A7%E6%88%98%E5%83%B5%E5%B0%B8), [PVZ Wiki](https://wiki.pvz1.com/doku.php?id=home) y [PvZ Tools](https://pvz.tools/memory/)). Usa backends portables como SDL2 y OpenGL ES 2.0 (con soporte de reserva en OpenGL 2.1 para escritorio).
* Este proyecto es solo con fines **educativos**, enfocándose en **técnicas de portabilidad**, modernización de motores y cómo adaptar la lógica de juegos clásicos a diferentes arquitecturas de hardware (Switch, 3DS, etc.).
* No comercial: Este proyecto no está afiliado, autorizado ni respaldado por PopCap Games o EA.
* Los íconos y logos de plataformas son creados por mí (wszqkzqk) con ayuda de herramientas de generación de imágenes AI y no son oficiales de PopCap/EA.
* Para jugar necesitas los archivos originales del juego, comprándolo en [EA](https://www.ea.com/games/plants-vs-zombies/plants-vs-zombies) o [Steam](https://store.steampowered.com/app/3590/Plants_vs_Zombies_GOTY_Edition/).

## Funciones principales

Basado en las implementaciones de [Patoke](https://github.com/Patoke/re-plants-vs-zombies) y [Headshotnoby](https://github.com/headshot2017/re-plants-vs-zombies) de PvZ GOTY:

* Sustituye el renderizado por SDL + OpenGL ES 2.0 (con fallback a OpenGL 2.1 en escritorio) y permite **redimensionar la ventana**.
* Código multiplataforma que reemplaza partes específicas de Windows.
* Sustituye DirectSound/BASS/FMod por [SDL Mixer X](https://github.com/WohlSoft/SDL-Mixer-X), compatible con archivos MO3 usando libopenmpt.
* Soporte para `main.pak` y optimización parcial de memoria para consolas.
* Compatible con datos de usuario originales de PvZ GOTY (`user*.dat`), incluyendo progreso, monedas y Zen Garden.
* Formato de guardado **portátil mid-level `.v4`** entre plataformas y exportable/importable en YAML para edición.
* Soporte multihilo con `std::thread` y sistema de archivos con `std::filesystem`.
* Soporte UTF-8 y reemplazo de strings anchos por `std::string`.
* Multilenguaje: chino, alemán, español, francés, italiano.
* Compatible con 32 y 64 bits, diferentes arquitecturas de CPU y endianness.

## Plataformas compatibles

| Plataforma | Ruta de datos                                        | Estado                              |
| ---------- | ---------------------------------------------------- | ----------------------------------- |
| Linux      | Ejecutable y app-data usuario                        | Funciona                            |
| Windows    | Ejecutable y app-data usuario                        | Funciona                            |
| macOS      | Ejecutable y app-data usuario                        | Funciona                            |
| BSD        | Ejecutable y app-data usuario                        | Funciona (FreeBSD probado)          |
| Haiku      | Ejecutable y app-data usuario                        | Parcial: sin música                 |
| Android    | `Android/data/io.github.wszqkzqk.pvzportable/files/` | Funciona                            |
| iOS/iPadOS | Directorio Documents (solo sideload)                 | Funciona                            |
| Switch     | `sdmc:/switch/PvZPortable`                           | Funciona, algunos homebrews fallan  |
| 3DS        | `sdmc:/3ds/PvZPortable`                              | En desarrollo, limitado por memoria |
| Web (WASM) | IndexedDB del navegador                              | Funciona, requiere servidor HTTP    |

Coloca `main.pak` y `properties/` junto al ejecutable para jugar. Los archivos se leen del directorio por defecto, y los datos guardados por usuario se almacenan en la ruta recomendada por el sistema operativo.

## Android / iOS

* Android: descarga APK desde [Releases](https://github.com/wszqkzqk/PvZ-Portable/releases), importa tus recursos del juego y lanza la app.
* iOS/iPadOS: descarga IPA sin firmar o compílala, usa sideload (AltStore, TrollStore o Xcode).

## Web

**[▶ Juega en el navegador](https://wszqkzqk.github.io/pvz-portable-wasm/pvz-portable.html)** — carga tus archivos legales del juego y empieza. Todo se queda en tu navegador y los datos se guardan localmente.

## Compatibilidad de versión

Compatible con Plants vs. Zombies **GOTY Edition 1.2.0.x** en inglés, Steam y versiones localizadas. Para mejor experiencia, usa el paquete 1.2.0.1073 EN.

## Guardado y formato `.v4`

* Datos de usuario globales: progreso, monedas, Zen Garden, totalmente compatibles con el juego original.
* Guardados mid-level `.v4`: portables entre plataformas, reemplazan antiguos `.dat` frágiles que dependen de la arquitectura.

## Contribución y licencia

* Licencia: [LGPL-3.0-or-later](https://www.gnu.org/licenses/lgpl-3.0.html).
* Los assets del juego pertenecen a PopCap/EA, esta licencia aplica solo al código.
* Framework SexyAppFramework usado y modernizado bajo LGPL-3.0.

Agradecimientos a los contribuyentes principales y a la comunidad por mantener el proyecto vivo.
