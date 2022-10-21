import functools


def main():
    acts_n = int(input())
    acts = {}
    acts_size = {}
    for _ in range(acts_n):
        line = input().split(',')
        acts[line[0]] = []
        acts_size[line[0]] = int(line[1])
    contend_n = int(input())
    contenders = []
    prefered_final = []
    rating = []
    penalty = []
    for _ in range(contend_n):
        inp = input().split(',')
        contenders.append(inp[0])
        prefered_final.append(inp[1])
        rating.append(int(inp[2]))
        penalty.append(int(inp[3]))
        acts[inp[1]].append((inp[0], int(inp[2]), int(inp[3])))

    def compare_by_penalty(left, right):
        return -(right[2] - left[2])

    def compare_by_rating(left, right):
        return right[1] - left[1]

    for contestants in acts.values():
        contestants.sort(key=functools.cmp_to_key(compare_by_penalty))
        contestants.sort(key=functools.cmp_to_key(compare_by_rating))

    final_list = []
    for finale_name, cont in acts.items():
        filtered_contends = cont[:acts_size[finale_name]]
        final_list += [x[0] for x in filtered_contends]

    final_list.sort()
    for f in final_list:
        print(f)

    return 0


if __name__ == '__main__':
    main()


#input
# 2
# cloud_developer,2
# cloud_hacker,3
# 5
# anonymous,cloud_hacker,6,0
# bjarne_stroustrup,cloud_developer,6,1
# julian_assange,cloud_hacker,5,100500
# bill_gates,cloud_developer,3,1
# guccifer,cloud_hacker,2,0

#result
# anonymous
# bill_gates
# bjarne_stroustrup
# guccifer
# julian_assange

#input
# 2
# ear_flying,1
# sun_bathing,1
# 3
# cheburashka,ear_flying,11,100
# dambo,ear_flying,10,0
# crocodile_gena,sun_bathing,11,10

# result
# cheburashka
# crocodile_gena