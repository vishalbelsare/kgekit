def strip_labels(filename):
    """Strips labels."""
    labels = []
    with open(filename) as f, open('processed_labels.txt', 'w') as f1:
        for l in f:
            l = l.replace(" .", '')
            l = l.replace(">\tskos:prefLabel\t", '')
            l = l.replace("<", '')
            f1.write(l)

if __name__ == '__main__':
    strip_labels(sys.argv[1])
