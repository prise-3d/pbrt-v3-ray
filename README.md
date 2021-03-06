# High contribution path study

## Description

Based on problem paths generated by the pbrt-v3 rendering engine. This project aims to convert the obtained paths into a file that can be included within a pbrt scene. The objective is to visualize these problem paths and understand their origin.

## How to use ?

### Build the project
```
mkdir build
cd build
cmake ..
make -j
```

### Use generated binary 
```bash
./topbrt --folder <folder-extracted-path> --output <output-folder>
```

_where:_
- `--folder`: is the folder where extracted paths data are available.

**Note:** paths extraction is available on specific branch of custom [pbrt-v3 version](https://github.com/prise-3d/pbrt-v3/tree/feature/rayStudy).

## Contributors

- Main contributor: [Christophe Renaud](https://www-lisic.univ-littoral.fr/article50-membre-39.html)
- [@jbuisine](https://github.com/jbuisine)

## License

[The MIT license](LICENSE)