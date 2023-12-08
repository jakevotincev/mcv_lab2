# Лабораторная работа 2

## Цель
Изучить основы оптимизации и векторизации алгоритмов компьютерного
зрения на базе процессорной системы ARM Cortex A57 MPCore + NEON. 

## Вариант
Сложение и вычитание двух изображений.

## Теория
Арифметические операции над изображениями выполняются попиксельно и покомпонентно. Для одинаково расположенных пикселей (r1; g1; b1) и (r2; g2; b2) их сумма равна (r1 + r2; g1 + g2; b1 + b2), а разность (r1 - r2; g1 - g2; b1 - b2).
В сложении сумма принимается равной 255, если она больше этого числа и минимальное значение 0 при разности.

## Описание программы
Реализовано 3 программы: сложение и вычитание без использования сторонних библиотек, с использованием opencv и с использованием библиотеки arm NEON.

### Сложение
```c++
void AddImage(uint8_t *dst, uint8_t const *src1, uint8_t const *src2, int width, int height)
{
    int index = 0;
    for (int y = 0; y < height * 3; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int t = src1[index] + src2[index];

            dst[index] = t > 255 ? 255 : t;

            index++;
        }
    }
}
```

### Вычитание
```c++
void SubImage(uint8_t *dst, uint8_t const *src1, uint8_t const *src2, int width, int height)
{
    int index = 0;
    for (int y = 0; y < height * 3; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int t = src1[index] - src2[index];

            dst[index] = t < 0 ? 0 : t;

            index++;
        }
    }
}
```

### Сложение neon
```c++
void AddImageNEON(uint8_t *dst, uint8_t const *src1, uint8_t const *src2, int width, int height)
{
    int const vectorNumberPerRow = width / 16;

    for (int y = 0; y < 3 * height; y++)
    {
        for (int i = 0; i < vectorNumberPerRow; i++)
        {

            uint8x16_t a = vld1q_u8(src1);
            uint8x16_t b = vld1q_u8(src2);

            uint8x16_t r = vqaddq_u8(a, b);

            vst1q_u8(dst, r);

            src1 += 16;
            src2 += 16;
            dst += 16;
        }
    }
}
```

### Вычитание neon
```c++
void SubImageNEON(uint8_t *dst, uint8_t const *src1, uint8_t const *src2, int width, int height)
{
    int const vectorNumberPerRow = width / 16;

    for (int y = 0; y < 3 * height; y++)
    {
        for (int i = 0; i < vectorNumberPerRow; i++)
        {

            uint8x16_t a = vld1q_u8(src1);
            uint8x16_t b = vld1q_u8(src2);

            uint8x16_t r = vqsubq_u8(a, b);

            vst1q_u8(dst, r);

            src1 += 16;
            src2 += 16;
            dst += 16;
        }
    }
}
```

## Результаты
Исходные картинки:
### Первая
![image](images/im1920_1.jpg)
### Вторая
![image](images/im1920_2.jpg)
### Сумма
![image](images/img1920_sum.png)
### Разница
![image](images/img1920_sub.png)

## Сравнение производительности
### Сложение neon
```c++
4k
estimated_time
5084162 us
avg 50841
```
```c++
full hd
estimated_time
1246649 us
avg 12466
```
```c++
960x960
estimated_time
542377 us
avg 5423
```
```c++
320X320
estimated_time
46306 us
avg 463
```
### Обычное сложение 
```c++
-O0 full hd
estimated_time
9153327 us
avg 91533
```
```c++
-o3 full hd
estimated_time
481470 us
avg 4814
```
```c++
-01 full hd
estimated_time
1475630 us
avg 14756
```
```c++
320
estimated_time
344510 us
avg 3445
```
```c++
960
estimated_time
4161422 us
avg 41614
```
```c++
4k
estimated_time
38352356 us
avg 383523
```


