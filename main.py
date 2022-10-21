import json
import datetime


def main():

    data_json = input()
    price_less = input()
    date_after = input()
    name_contains = input()
    price_greater = input()
    date_before = input()

    data_json = json.loads(data_json)
    price_less = int(price_less.split()[1])

    date_after_tmp = date_after.split()[1].split('.')
    date_after = datetime.datetime(int(date_after_tmp[2]), int(date_after_tmp[1]), int(date_after_tmp[0]))
    name_contains = name_contains.split()[1]

    price_greater = int(price_greater.split()[1])
    date_before_tmp = date_before.split()[1].split('.')
    date_before = datetime.datetime(int(date_before_tmp[2]), int(date_before_tmp[1]), int(date_before_tmp[0]))

    save_data = []
    for id in data_json:
        datet = id["date"].split('.')
        date = datetime.datetime(int(datet[2]), int(datet[1]), int(datet[0]))
        print(int(id["price"]))
        if id["name"].lower().find(name_contains) < 0:
            continue
        if (price_greater > int(id["price"])) or (int(id["price"]) > price_less):
            continue
        if (date_after > date) or (date > date_before):
            continue
        save_data.append(id)

    print(save_data)

if __name__ == '__main__':
    main()

#input
# [{"id": 1, "name": "Asus notebook","price": 1564,"date": "23.09.2021"},{"price": 2500, "id": 3, "date": "05.06.2020", "name": "Keyboardpods" }, {"date": "23.09.2021", "name": "Airpods","id": 5, "price": 2300}, {"name": "EaRPoDs", "id": 2, "date": "01.01.2022", "price": 2200}, { "id": 4, "date": "23.09.2021", "name": "Dell notebook",  "price": 2300}]
# PRICE_LESS_THAN 2400
# DATE_AFTER 23.09.2021
# NAME_CONTAINS pods
# PRICE_GREATER_THAN 2200
# DATE_BEFORE 02.01.2022

#expected result
# [{'date': '23.09.2021', 'name': 'Airpods', 'id': 5, 'price': 2300}, {'name': 'EaRPoDs', 'id': 2, 'date': '01.01.2022', 'price': 2200}]