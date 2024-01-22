
def main():
    p = int(input("p = "))
    g = int(input("g = "))
    a = int(input("a = "))

    product = 1
    for i in range(a):
        product = (product * g) % p
    
    print("%d^%d = %d" % (g, a, product))

if __name__ == "__main__":
    main()

