<div dir="rtl">
# مقدمه

این ریپوزیتوری شامل سورس سایت [Movies Api](http://moviesapi.ir/) می‌باشد. برای ساخت این سایت از فریم‌ورک لاراول ۵.۳ استفاده شده است. «فیلم‌ها‌» برای حل مشکل دو گروه از توسعه‌دهندگان به وجود آماده است: گروه اول) کسانی هستند که به تازگی یادگیری یک زبان جدید مثل android یا ios را شروع کرده‌اند و برای تمرین نیاز به یک وب سرویس کامل و بدون پیچیدگی‌های رایج (مثل اجباری بودن api_key و … ) دارند. گروه دوم) توسعه‌دهندگان حرفه‌ای می‌باشند که برای مقاله‌های آموزشی و یا آزمودن روش‌های مختلف نیاز به یک وب سرویس کامل دارند، البته این گروه توانایی رفع نیازشان را دارند اما با استفاده از «فیلم‌ها» می‌توانند در وقت خود صرفه‌جویی کنند.
در آینده‌‌ متدهای بیشتری به وب سرویس اضافه می‌شود و حتی توسعه‌دهندگان می‌توانند روش‌های authentication مثل jwt را امتحان کنند. همچنین از همکاری توسعه‌دهنگان دیگر نیز استقبال می‌شود.

# نیازمندی‌ها

+ [نیازمندی‌های فریم‌ورک لاراول](https://laravel.com/docs/5.4#server-requirements)
+ فعال بودن افزونه fileinfo در PHP

# راهنمای نصب

1. ریپو را کلون کنید.

<pre dir="ltr">https://github.com/abbas-oveissi/MoviesApi_ir.git</pre>

2. فایل env.example. را در پوشه‌ی local به env. تغییر داده و اطلاعات مربوط به خودتان را برای تنظیمات DB و APP وارد کنید ( برای استفاده از فرم تماس، اطلاعات Mail را هم باید وارد کنید. )

3. در پوشه‌ی همان پوشه‌ی local دستورات زیر را اجرا کنید.
4. دستور نصب dependencyها

	composer install
5.  دستور ساخت جدول‌های مورد نیاز پروژه در دیتابیس
	php artisan migrate
6. دستور ساخت application key
	php artisan key:generate

7. اگر میخواهید اطلاعات ۱۵ فیلم برتر به همراه ژانرشان در جدول‌ها باشد، دستور زیر را هم اجرا کنید.

	php artisan db:seed
