# Laplace MPU (v2)

<div style="position: relative; width: 100%; padding-top: 56.25%;">
  <img src="docs/banner-v2.png" alt="Laplace Matrix Coprocessor's Banner" style="position: absolute; top: 0; left: 0; width: 100%; height: 100%; object-fit: cover;">
</div>

## How to compile

```
gcc -std=c99 -Iapp -Ilib app/main.c lib/laplace.s -o laplace
```

* Observations: You must include both `app/` and `lib/` folder to compile it.
