# PvZ-Portable

> Reimplementación multiplataforma de Plants vs. Zombies: GOTY Edition impulsada por la comunidad.  
> Objetivo: experiencia **100% auténtica** en cualquier plataforma.

---

## ⚠️ Aviso legal importante

- Este repositorio **NO incluye** ningún recurso con derechos de autor (imágenes, música, fuentes) propiedad de PopCap Games o Electronic Arts.
- Para jugar **necesitas obligatoriamente** tu propia copia de `main.pak` y la carpeta `properties/` obtenidas de una **copia comprada legalmente** de Plants vs. Zombies: GOTY Edition.
- El código es una reimplementación manual basada en documentación pública e investigación de la comunidad. El autor **nunca realizó ingeniería inversa** del ejecutable original.
- Este proyecto es **sin fines de lucro** y tiene únicamente propósitos educativos: técnicas de portabilidad multiplataforma, modernización de motores y estudio de arquitecturas de hardware (Switch, 3DS, etc.).
- No está afiliado, autorizado ni respaldado por PopCap Games ni Electronic Arts.

Compra el juego en [EA](https://www.ea.com/games/plants-vs-zombies/plants-vs-zombies) o [Steam](https://store.steampowered.com/app/3590/Plants_vs_Zombies_GOTY_Edition/).

---

## 🎮 Jugar en el navegador

**[▶ Jugar online](https://wszqkzqk.github.io/pvz-portable-wasm/pvz-portable.html)** — carga tus recursos del juego (ZIP o `main.pak` + `properties/`), haz clic en *Start Game* y listo. Los archivos **nunca se suben a ningún servidor** — todo ocurre localmente en tu navegador.

---

## ✨ Características

Basado en las implementaciones de [@Patoke](https://github.com/Patoke/re-plants-vs-zombies) y [@headshot2017](https://github.com/headshot2017/re-plants-vs-zombies), con las siguientes mejoras:

- **Renderizado moderno**: OpenGL ES 2.0 con fallback a OpenGL 2.1 de escritorio, usando SDL2. Permite **redimensionar la ventana**, cosa imposible en el juego original.
- **Audio multiplataforma**: DirectSound/BASS/FMod reemplazados por [SDL Mixer X](https://github.com/WohlSoft/SDL-Mixer-X) con soporte para MO3 via libopenmpt.
- **Soporte de `main.pak`**: compatible con los archivos de recursos originales sin necesidad de extraerlos.
- **Compatibilidad de guardados globales**: los archivos `user*.dat` son 100% compatibles con el juego original de PC, incluyendo corrección del bug del año 2038.
- **Nuevo formato de guardado de nivel `.v4`**: portátil entre plataformas y arquitecturas (x86, ARM, RISC-V, LoongArch). Exportable/importable a YAML para edición manual.
- **Soporte multilenguaje**: inglés, chino, alemán, español, francés e italiano (ediciones GOTY oficiales).
- **Multiarquitectura**: i686, x86_64, aarch64, riscv64, loongarch64, entre otras.
- **Threading y filesystem modernos**: `std::thread` y `std::filesystem` de C++20.
- **Rutas Unicode** en todas las plataformas.
- **Soporte de endianness**: little-endian y big-endian (teóricamente; big-endian sin probar por falta de hardware).
- **Correcciones opcionales de bugs** del juego original (ver opción `DO_FIX_BUGS`).

---

## 📱 Plataformas soportadas

| Plataforma | Ruta de datos | Estado |
|---|---|---|
| Linux | Directorio del ejecutable (recursos); appdata por usuario (escritura) | ✅ Funciona |
| Windows | Directorio del ejecutable (recursos); appdata por usuario (escritura) | ✅ Funciona |
| macOS | Directorio del ejecutable (recursos); appdata por usuario (escritura) | ✅ Funciona |
| BSD (FreeBSD, etc.) | Igual que Linux | ✅ Funciona |
| Haiku | Igual que Linux | ⚠️ Funciona parcialmente (sin música) |
| Android | `Android/data/io.github.wszqkzqk.pvzportable/files/` | ✅ Funciona |
| iOS / iPadOS | Carpeta Documents de la app (Files app) | ✅ Funciona (solo sideload; IPA sin firmar) |
| Nintendo Switch | `sdmc:/switch/PvZPortable/` | ✅ Funciona en hardware real. ❌ Kenji-NX crashea al arrancar ([#148](https://github.com/wszqkzqk/PvZ-Portable/issues/148)) |
| Nintendo 3DS | `sdmc:/3ds/PvZPortable/` | 🚧 En desarrollo. Old 3DS probablemente sin suficiente RAM. New 3DS funciona con dificultad |
| Web (WASM) | IndexedDB del navegador (guardados); recursos cargados en runtime | ✅ Funciona (requiere servidor HTTP) |

---

## 🚀 Instalación y uso

### Requisitos de recursos del juego

Necesitas los siguientes archivos de una copia legal de PvZ GOTY Edition:

```
main.pak
properties/
```

Colócalos junto al ejecutable `pvz-portable`. También puedes usar los datos extraídos del `.pak` si lo prefieres.

**Versiones de assets compatibles:**

| Versión | Idioma | Notas |
|---|---|---|
| 1.2.0.1073 | EN | ✅ Recomendada — mejor experiencia de inicio |
| 1.2.0.1096 | EN | ✅ Steam — algunos textos cosméticos incorrectos (corregibles vía `default.xml`) |
| 1.2.0.1093 | DE/ES/FR/IT | ✅ Funciona — mismos textos cosméticos |
| 1.1.0.1056 | ZH | ⚠️ Puede congelarse mostrando el banner de "oleada grande de zombies" |

### Rutas de datos por plataforma

Los recursos se buscan en el directorio del ejecutable. Los datos de escritura (guardados, caché, configuración) se almacenan en la ruta de appdata recomendada por el SO:

- **Linux**: `~/.local/share/io.github.wszqkzqk/PvZPortable/`
- **Windows**: `%APPDATA%\io.github.wszqkzqk\PvZPortable\`
- **macOS**: `~/Library/Application Support/io.github.wszqkzqk/PvZPortable/`

Dentro de esa carpeta encontrarás:
- `userdata/` — Guardados del jugador
- `cache64/` o `cache32/` — Cachés compilados locales (no portátiles entre plataformas)
- `registry.regemu` — Configuración del juego

### Parámetros de línea de comandos

```bash
# Cambiar directorio de recursos
pvz-portable -resdir="<ruta>"

# Cambiar directorio de guardados
pvz-portable -savedir="<ruta>"
```

> ⚠️ Usa siempre el formato `-param="<ruta>"`. No se soportan valores separados por espacios.

---

## 🤖 Android

1. Descarga el APK desde [Releases](https://github.com/wszqkzqk/PvZ-Portable/releases) o compílalo tú mismo.
2. Instala el APK (necesitas habilitar "Instalar de fuentes desconocidas").
3. Al abrir por primera vez, aparecerá una pantalla de importación de recursos.
4. Selecciona un ZIP con `main.pak` + `properties/`, o una carpeta que los contenga.
5. Pulsa **Start Game**.

Para gestionar datos después: mantén pulsado el icono de la app → **Manage Data**.

**Notas:**
- Requiere Android 9.0+. El APK precompilado es solo arm64-v8a.
- Todos los datos en `Android/data/io.github.wszqkzqk.pvzportable/files/`. Sin permisos extra de almacenamiento.
- El port usa SDL2 para mapear eventos táctiles a mouse — jugable pero sin optimizaciones específicas para móvil.

---

## 🍎 iOS / iPadOS

1. Descarga la IPA sin firmar desde [Releases](https://github.com/wszqkzqk/PvZ-Portable/releases) o compílala con `ios/build-ios.sh`.
2. Instala via [AltStore](https://altstore.io/), [TrollStore](https://github.com/opa334/TrollStore) o Xcode con Apple ID gratuito.
3. Coloca `main.pak` y `properties/` en la carpeta Documents de la app (visible en la app Archivos como "PvZ Portable").

**Notas:**
- Requiere iOS 15.0+ (arm64).
- Las firmas con Apple ID gratuito expiran a los 7 días. TrollStore es permanente.
- Mismo comportamiento táctil que Android.

---

## 🌐 WebAssembly (Navegador)

**[▶ Jugar online](https://wszqkzqk.github.io/pvz-portable-wasm/pvz-portable.html)**

O descarga la build WASM desde [Releases](https://github.com/wszqkzqk/PvZ-Portable/releases) y sírvela localmente:

```bash
python3 -m http.server
```

> ⚠️ No puede abrirse directamente como archivo local — el navegador bloquea esto por seguridad.

Los guardados se almacenan en IndexedDB del navegador y pueden exportarse/importarse como ZIP.

---

## 🎮 Nintendo Switch

Los recursos van en `sdmc:/switch/PvZPortable/`:

```
sdmc:/switch/PvZPortable/
├── main.pak
├── properties/
└── userdata/
```

**Estado actual:** Funciona en hardware real. El emulador **Kenji-NX crashea al arrancar** — esto es un bug conocido del emulador, no del port (ver [issue #148](https://github.com/wszqkzqk/PvZ-Portable/issues/148)). Se recomienda usar **Ryujinx** o hardware real.

---

## 🏗️ Compilación

### Dependencias

| Categoría | Paquetes |
|---|---|
| Build | `CMake`, `Ninja`, compilador C++20 (gcc/clang/MSVC) |
| Gráficos | `OpenGL ES 2.0` o `OpenGL 2.1+` |
| Audio | `libopenmpt`, `libogg`, `libvorbis`, `mpg123` |
| Imágenes | `libpng`, `libjpeg-turbo` |
| Ventana/Input | `SDL2` |

#### Arch Linux
```bash
sudo pacman -S --needed base-devel cmake libjpeg-turbo libogg libopenmpt libpng libvorbis mpg123 ninja sdl2-compat
```

#### Debian / Ubuntu
```bash
sudo apt install cmake ninja-build libogg-dev libjpeg-dev libopenmpt-dev libpng-dev libvorbis-dev libmpg123-dev libsdl2-dev
```

#### Windows (MSYS2 UCRT64)
```bash
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-libjpeg-turbo mingw-w64-ucrt-x86_64-libopenmpt mingw-w64-ucrt-x86_64-libogg mingw-w64-ucrt-x86_64-libpng mingw-w64-ucrt-x86_64-libvorbis mingw-w64-ucrt-x86_64-mpg123 mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-SDL2
```

#### macOS (Homebrew)
```bash
brew install cmake dylibbundler jpeg-turbo libogg libopenmpt libpng libvorbis mpg123 ninja sdl2
```

### Compilar

```bash
# Debug (por defecto)
cmake -G Ninja -B build
cmake --build build

# Release (recomendado para jugar)
cmake -G Ninja -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Release con optimizaciones nativas de CPU
cmake -G Ninja -B build -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_FLAGS="-march=native" -DCMAKE_CXX_FLAGS="-march=native"
cmake --build build
```

### Opciones de CMake

| Opción | Por defecto | Descripción |
|---|---|---|
| `PVZ_DEBUG` | `OFF` (ON en Debug) | Activa teclas de truco, pantallas de debug y otras herramientas de desarrollo |
| `LIMBO_PAGE` | `ON` | Habilita acceso a la "limbo page" con niveles ocultos |
| `DO_FIX_BUGS` | `OFF` | Aplica correcciones de la comunidad para bugs del juego oficial (ver notas abajo) |
| `CONSOLE` | `OFF` (ON en Debug) | Muestra ventana de consola (solo Windows) |
| `BUILD_STATIC` | `OFF` | Linkeo estático para ejecutable standalone (solo Windows con MinGW) |

Ejemplo — build Release con teclas de truco activadas:
```bash
cmake -G Ninja -B build -DCMAKE_BUILD_TYPE=Release -DPVZ_DEBUG=ON
```

---

## 💾 Compatibilidad de guardados

PvZ-Portable usa dos tipos de archivos de guardado:

### 1. Datos globales del usuario (`users.dat`, `user1.dat`, etc.)
- Perfil, progreso de aventura, monedas, Jardín Zen, etc.
- **100% compatible** con el formato original de PC.
- Portátil por diseño (usa serialización explícita).

### 2. Estados de nivel a mitad de partida
| Formato | Archivo | Portátil | Notas |
|---|---|---|---|
| `.v4` | `game1_0.v4` | ✅ Sí | Formato nuevo TLV — compartir entre plataformas es totalmente seguro |
| `.dat` | `game1_0.dat` | ❌ No | Legacy — volcado de memoria raw. No compartir entre plataformas distintas |

Al cargar, el juego prefiere `.v4`. Si solo existe `.dat`, lo migra automáticamente a `.v4` y borra el `.dat` original.

### ¿Por qué un nuevo formato?

El `.dat` original vuelca memoria raw, lo que rompe entre:
- Arquitecturas distintas (x86 vs ARM vs RISC-V) — diferente alineación
- Builds 32-bit vs 64-bit — tamaños de puntero distintos
- Compiladores (MSVC vs GCC/Clang) — padding y packing distintos

El formato `.v4` serializa campo a campo con etiquetas TLV de tipo-longitud-valor, garantizando compatibilidad cruzada total.

### Herramienta de edición de guardados

```bash
# Ver información básica
python scripts/pvzp-v4-converter.py info ~/.local/share/io.github.wszqkzqk/PvZPortable/userdata/game1_13.v4

# Exportar a YAML para edición
python scripts/pvzp-v4-converter.py export game1_13.v4 nivel.yaml

# Importar desde YAML (¡haz backup primero!)
python scripts/pvzp-v4-converter.py import nivel.yaml game1_13.v4
```

---

## 🤝 Contribuir

Sigue las convenciones de nomenclatura del SexyAppFramework:

- `m` → miembro de clase (ej: `mBoard`, `mZombies`)
- `the` → parámetro de función (ej: `theX`, `theWidget`)
- `a` → variable local (ej: `aZombie`, `aCount`)

Algunos miembros son públicos por diseño — es una característica del framework, no un bug.

**Nota importante:** No se acepta código generado directamente por ingeniería inversa del ejecutable original.

---

## 🐛 Correcciones de bugs incluidas (`DO_FIX_BUGS=ON`)

<details>
<summary>Ver lista completa</summary>

- Corrige doble drop de sol/objetos del zombie bungee en modo I, Zombie.
- Los Gargantuars controlados mentalmente atacan zombies enemigos en lugar de plantas.
- Los Gargantuars controlados mentalmente lanzan Imps también controlados mentalmente (con correcciones de escala, salud y dirección).
- Los Gargantuars controlados mentalmente pueden romper vasijas en modo Scary Potter.
- Los zombies Pea/Gatling Head controlados mentalmente disparan hacia adelante, no hacia atrás.
- Los zombies Jalapeno/Squash controlados mentalmente dañan zombies enemigos en lugar de plantas.
- Correcciones de coordenadas para el Squash controlado mentalmente rastreando y aplastando zombies enemigos.
- El Jalapeno controlado mentalmente elimina correctamente las habilidades del Dr. Zomboss (bola de hielo/fuego) y la lógica de escaleras.
- Sincronización de animaciones del Dancer Zombie (corrige bug de desplazamiento "Maid").
- Corrección del glitch visual del brazo del Ladder Zombie.
- Corrección del rango de ataque y convocatoria del Dr. Zomboss para niveles de 6 carriles (Pool).

</details>

---

## 📜 Licencia

**Copyright (C) 2026 Zhou Qiankang**

Este proyecto está licenciado bajo [GNU Lesser General Public License v3.0 o posterior (LGPL-3.0-or-later)](https://www.gnu.org/licenses/lgpl-3.0.html).

- `LICENSE` — texto LGPL-3.0
- `COPYING` — texto GPL-3.0 (referenciado por LGPL-3.0)
- La IP del juego original (Plants vs. Zombies) pertenece a PopCap/EA. Esta licencia aplica **solo al código** de este repositorio.
- El directorio `SexyAppFramework` puede contener código basado en el PopCap Games Framework (© 2005-2009 PopCap Games, Inc.), heavily refactorizado bajo LGPL-3.0-or-later.

---

## 🙏 Agradecimientos

- **[@headshot2017](https://github.com/headshot2017)** — Por implementar casi completamente el soporte de 64 bits y el backend OpenGL inicial de función fija.
- **[@Patoke](https://github.com/Patoke)** — Por la increíble reimplementación inicial de PvZ GOTY.
- **[@rspforhp](https://github.com/octokatherine)** — Por el trabajo en la versión 0.9.9.
- **[@ruslan831](https://github.com/ruslan831)** — Por archivar la reimplementación de la versión 0.9.9.
- **PopCap Games** — Por crear el juego y liberar su framework con licencia permisiva.
- **El equipo de SDL** — Por la increíble librería multiplataforma que impulsa este port.
- **El equipo de OpenMPT** — Por libopenmpt, que permite reproducción de alta calidad del formato MO3.
- A todos los contribuidores que han trabajado y siguen trabajando en este proyecto.
