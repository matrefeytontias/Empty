# Generate the swizzling.inl file that defines all the coordinate
# swizzling functions for vectors

numbers = "0123"
coords = ["xyzw", "rgba", "stpq"]


def hasDuplicates(a):
    seen = set()
    for x in a:
        if x in seen:
            return True
        seen.add(x)
    return False

# k : number of components in the ref vec
# n : number of components in the original vec


def permutations(k, n):
    r = set()
    for i in range(n ** k):
        base = i
        refcoords = ""
        for j in range(k):
            refcoords = refcoords + numbers[base % n]
            base //= n
        r.add(refcoords)
    return r


with open("swizzling.inl", "w") as f:
    f.write("// This file is auto-generated. Do not edit !\n\n")

    seen = set()
    for n in range(2, 5):
        if n == 3:
            f.write("#ifdef VEC_HAS_Z\n")
        elif n == 4:
            f.write("#ifdef VEC_HAS_W\n")
        for k in range(2, 5):
            permut = permutations(k, n) - seen
            seen = seen.union(permut)
            permuts = sorted(permut)
            for indices in permuts:
                for letters in [[coords[i][int(k)] for k in indices] for i in range(len(coords))]:
                    if hasDuplicates(indices):
                        f.write("_vec" + str(k) + "ref<const T> " + "".join(letters) +
                                "() const { return _vec" + str(k) + "ref(" + ",".join(letters) + "); }\n")
                    else:
                        f.write("_vec" + str(k) + "ref<T> " + "".join(letters) +
                                "() { return _vec" + str(k) + "ref(" + ",".join(letters) + "); }\n")
    f.write("#endif\n#endif\n")
