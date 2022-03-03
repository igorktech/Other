


/*function getDayInfo(str){
    // формирование объекта типа Date
    var data =   new Date(Number(str.substring(6,10)), Number(str.substring(3,5))-1, Number(str.substring(0,2))) ;
    //массив дней
    var days = [
        'Воскресенье',
        'Понедельник',
        'Вторник',
        'Среда',
        'Четверг',
        'Пятница',
        'Суббота'
      ];
    //массив месяцев
    var month = [
        'Января ',
        'Февраля ',
        'Марта ',
        'Апреля ',
        'Мая ',
        'Июня ',
        'Июля ',
        'Августа ',
        'Сентября ',
        'Октября ',
        'Ноября ',
        'Декабря '
    ];
    //вычисление номера недели
    var weekNum = Math.ceil((data.getDate()+ data.getDay()-1)/ 7);
    var result = days[data.getDay()]+', '+weekNum+' неделя '+month[data.getMonth()]+data.getFullYear()+' года';
    return result;
}*/


//Сайт с формулой для вечного календаря
//http://slovesnov.users.sourceforge.net/index.php%3Fcalendar_formula,russian

function getDayInfo(str){
    //массив дней
    var days = [
        'Воскресенье',
        'Понедельник',
        'Вторник',
        'Среда',
        'Четверг',
        'Пятница',
        'Суббота'
      ];
    //массив месяцев
    var months = [
        'Января ',
        'Февраля ',
        'Марта ',
        'Апреля ',
        'Мая ',
        'Июня ',
        'Июля ',
        'Августа ',
        'Сентября ',
        'Октября ',
        'Ноября ',
        'Декабря '
    ];

    var month = Number(str.substring(3,5));
    var day = Number(str.substring(0,2));
    var year = Number(str.substring(8,10));
    var fullYear = Number(str.substring(6,10));
    var century =Number(str.substring(6,8));

    //номер месяца, где Март 1, Январь 11, Февраль 12 (для вечного календаря)
    var m = month-2;
    if(m<=0){
      m = m+12;
    }

    //год для вечного календаря
    var y = year;
    if((month == 1)||(month == 2)){
        if(year ==0 ){
            y = 99;
        }
        else{
            y = year-1;
        }
    }
    //вычисление номера дня недели по вечному календарю
    var week = (day+Math.floor((13*m-1)/5)+Math.floor(5*y/4)+Math.floor(-7*century/4)) % 7;
    //если остаток от деления отрицательный, прибавляется 7
    if(week<0){
       week = week + 7;
    }
    var dayofWeek = days[week];
    //вычисление номера недели
    var weekNum = Math.ceil((Number(str.substring(0,2))+ week)/ 7);
    //результат
    var result = dayofWeek+', '+weekNum+' неделя '+months[month-1]+fullYear+' года';
    return result;
};
console.log(getDayInfo("29.02.2024"));