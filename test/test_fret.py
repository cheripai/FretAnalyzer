from subprocess import call

def main():
    for i in range(1, 3):
        call(['python', '../src/fret.py', '-i', 'data' + str(i) + '.txt', '-o', 'results.txt'])
        with open('results.txt') as f:
            results = f.readlines()
        with open('data' + str(i) + '.txt') as f:
            answers = f.readlines()
        assert results[0] == answers[3], 'Incorrect answer'
        assert results[1] == answers[4], 'Incorrect answer'
    
    call(['rm', 'results.txt'])
    print("All tests valid.")

if __name__ == '__main__':
    main()
