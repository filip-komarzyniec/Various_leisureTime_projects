# 4 / 2 / 2 * 5 + 6 + 7 / 2 * 2


def main():
	express1 = []
	express = input("Podaj działanie, jakie chcesz wykonać, poszczególne elementy rozdziel spacjami\n")
    
	express = express.split(' ')
	if "/" not in express and "*" not in express and "%" not in express:
		pass
		# tylko dodawanie i odejmowanie, przejdź dalej
	elif "+" not in express and "-" not in express and "%" not in express:
		pass
		# tylko mnożenie i dzielenie, przejdź dalej
	else:
		# jeśli są mieszane znaki
		# najpierw modulo, jeśli jest 
		express_it = iter(express)
		if "%" in express:
			for ind, i in enumerate(express_it):
				prev = i
				curr = next(express_it)
				nxt = next(express_it)
				if curr == "/" or curr=="*":
					express.remove(curr)
					express.remove(nxt)
					temp = prev / nxt
					express[ind] = temp
			express_it = iter(express) 	# tworzę iterator, jeśli jest modulo i go wykozrystam
		express_it2 = iter(express)
		for ind, i in enumerate(express_it):
			if i == "/":
				nxt = next(express_it)
				temp = float(prev) / float(nxt)
				express1.append(temp)
				try:
					c = next(express_it)
					while c == "*":
						temp2 = temp * float(next(express_it))
						express1.pop()
						express1.append(temp2)
						try:
							c = next(express_it)
							temp = temp2
							prev = next(express_it2)
							prev = next(express_it2)
						except StopIteration as e:
							break
				except StopIteration as e:
					break
				prev = next(express_it2)		#trzeba, bo nxt
				prev=next(express_it2)
				# tutaj rekurencja od if i =="/" chyba sama wgl wystarczy, bez kolejnego ifa, do momentu
			if i == "*":
				nxt = next(express_it)
				temp = float(prev) * float(nxt)
				express1.append(temp)
				c = next(express_it)
				while c == "/":
					temp2 = temp / float(next(express_it))
					express1.pop()
					express1.append(temp2)
					try:
						c = next(express_it)
						temp = temp2
						prev = next(express_it2)
						prev = next(express_it2)
					except StopIteration as e:
						break
				prev = next(express_it2)		#trzeba, bo nxt
				prev=next(express_it2)
				# tutaj rekurencja od if i =="*"
			if i!="*" and i!="/":
				if ind != 0:
					express1.append(prev)
			prev = next(express_it2)
			#dalej powinny być już tylko dodawania albo odejmowania 
		express1.append(express[len(express)-1])			# dodaję ostatni element listy
				
	# tu już same dodawania i odejmowania
	print("test"+str(express1))
	if express1:
		express_it = iter(express1)	# tu moja zmiana późno w nocy
	else:
		express_it = iter(express)	# jak nie ma różnych znaków
	res = float(next(express_it))
	while True:
		try:
			b = next(express_it)
			if b == "+":
				c = float(next(express_it))
				res = res + c
			elif b == "/":
				c = float(next(express_it))
				res = res / c
			elif b == "%":
				c = float(next(express_it))
				res = res % c
			elif b == "-":
				c = float(next(express_it))
				res = res - c
			elif b == "*":
				c = float(next(express_it))
				res = res * c
		except StopIteration as e:
			print("Twój wynik to: " + str(res)+"\n")
			mind = input("chcesz wykonać kolejne działanie? [T/n]")
			if mind == "T":
				main()
				break
			else: 
				print("Dziękuję, do widzenia")
				break

if __name__ == "__main__":
	main()
	
     
