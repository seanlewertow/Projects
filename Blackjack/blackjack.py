from p1_random import P1Random

rng = P1Random()


gameContinue = True
gameCount = 0

num_playerWins = 0
num_dealerWins = 0
num_ties = 0

choice = 0

temp = 0

wrongIntMenu = True

player_win_percentage = 0

while gameContinue:
    if choice == 4:
        break
    selectChoices = True

    playing = True

    playerHand = 0
    dealerHand = 0
    gameCount += 1
    print("START GAME #", gameCount)
    wrongIntMenu = True

    while playing:
        # temp = rng.next_int(13)
        if wrongIntMenu:
            card = rng.next_int(13) + 1
        # print("---card was---", temp)
        # card = temp + 1
        if wrongIntMenu:
            if card == 1:
                print("\nYour card is a ACE!")
            elif 2 <= card <= 10:
                print("\nYour card is a", str(card) + "!")
            elif card == 11:
                card = 10
                print("\nYour card is a JACK!")
            elif card == 12:
                card = 10
                print("\nYour card is a QUEEN!")
            elif card == 13:
                card = 10
                print("\nYour card is a KING!")
            playerHand += card
            print("Your hand is:", playerHand, "\n")

        if playerHand == 21:
            print("BLACKJACK! You win!\n")
            num_playerWins += 1
            break
        elif playerHand > 21:
            print("You exceeded 21! You lose.\n")
            num_dealerWins += 1
            break

        wrongIntMenu = True
        # choices
        print("1. Get another card")
        print("2. Hold hand")
        print("3. Print statistics")
        print("4. Exit\n")

        choice = int(input("Choose an option: "))
        while selectChoices:
            if choice == 1:
                wrongIntMenu = True
                selectChoices = False
            elif choice == 2:
                wrongIntMenu = True
                playing = False
                dealerHand = rng.next_int(11) + 16
                # temp = rng.next_int(11)
                # print("---DEALER TEMP card was---", temp)
                # dealerHand = temp + 16
                # print("---DEALER card was---", dealerHand)

                if dealerHand > 21:
                    print("Dealer's hand: ", dealerHand)
                    print("Your hand is: ", playerHand)
                    print("\nYou win!\n")
                    num_playerWins += 1
                    selectChoices = False
                elif dealerHand == playerHand:
                    print("Dealer's hand: ", dealerHand)
                    print("Your hand is: ", playerHand)
                    print("It's a tie! No one wins!")
                    num_ties += 1
                    selectChoices = False
                elif (21 - dealerHand) < (21 - playerHand):
                    print("Dealer's hand: ", dealerHand)
                    print("Your hand is: ", playerHand)
                    print("\nDealer wins!\n")
                    num_dealerWins += 1
                    selectChoices = False
                elif (21 - dealerHand) > (21 - playerHand):
                    print("You win!")
                    num_playerWins += 1
                    selectChoices = False

            elif choice == 3:
                player_win_percentage = float(num_playerWins * 100.0 / (gameCount - 1))
                print("Number of Player wins: ", num_playerWins)
                print("Number of Dealer wins: ", num_dealerWins)
                print("Number of tie games: ", num_ties)
                print("Total # of games played is: ", gameCount - 1)
                print("Percentage of Player wins: ", player_win_percentage, "%")
                wrongIntMenu = False
                selectChoices = False
            elif choice == 4:
                playing = False
                gameContinue = False
            elif choice < 1 or choice > 4:
                print("Invalid input!")
                print("Please enter an integer value between 1 and 4.\n")
                wrongIntMenu = False
                selectChoices = False

        # if choice == 2 or choice == 4:
        #    break
