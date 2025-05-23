# Laplace MPU (v2)

![Laplace, Matrix Processor Unit](docs/banner.png)

## How to compile

```
gcc -Ilib -Isrc app/main.c lib/laplace.s -o laplace
```

* Observations: You must include both `app/` and `lib/` folder to compile it.
