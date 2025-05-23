# Laplace MPU (v2)

![Laplace MPU banner (v2)](docs/banner-v2-readme.png)

## How to compile

```
gcc -std=c99 -Iapp -Ilib app/main.c lib/laplace.s -o laplace
```

* Observations: You must include both `app/` and `lib/` folder to compile it.
