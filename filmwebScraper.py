import io
import typing

from requests import Session
from bs4 import BeautifulSoup, element
from time import sleep
from pandas import DataFrame
from typing import Union


def next_actor_page(base_url: str, start_page: int=1, max_page: int=1) -> str:
	"""Generator function that produces links to the next filmweb subpages in a given range
	Args:
		base_url: starting url
		start_page: starting subpage
		max_page: last subpage in range
	Yields:
		next_url: url to visit next
	"""
	next_url = base_url
	for i in range(start_page, max_page+1):
		if "&page" in next_url:
			ind = next_url.rfind("=") + 1
			next_url = next_url[:ind] + "{}".format(int(next_url[ind:]) + 1)
		else:
			next_url += f"&page={start_page}"
		yield next_url

def a_has_h3_parent(elem: element) -> bool:
	"""helper function to search HTML DOM for an anchor that has a h3 parent
	Args:
		elem: DOM element
	"""
	return True if elem.name == "a" and elem.parent.name == "h3" else False

def cache_response(res: str, filename: str) -> None:
	"""helper function saving requested HTML page to file. This way total
	number of HTTP requests is minimized
	Args:
		res: result string from HTTP request
		filename: name for the file to be created
	"""
	with open(filename, "w") as f:
		f.write(res)

def populate_dct(key, dct: dict, *vals) -> None:
	"""helper function to populate dictionary with given key and values.
	Each key is mapped to the list of values
	Args:
		key: key to insert into dict
		dct: dict to populate
		*vals: arbitrary list of values to insert into the dict under a given key

	"""
	if key not in dct:
		dct[key] = list(vals)
	else:
		dct[key] += vals

def get_actor_rating(soup: BeautifulSoup) -> typing.Iterable[str, str]:
	"""helper function extracting from HTML DOM rating and number of votes
	based on which rating was derived.
	Args:
		soup: BeautifulSoup object to process
	Returns:
		rating: rating of an actor
		votes: number of user votes that made the rating
	"""
	rating_div = soup.find("div", class_="personRating__rating")
	rating_elements = list(rating_div.descendants)
	rating = rating_elements[1].replace(",", ".")
	votes = rating_elements[3].string
	return rating, votes

def get_actor_awards(soup: BeautifulSoup) -> str:
	"""Helper function extracting from HTML DOM number of awards for a given actor
	Arguments:
		soup: BeautifulSoup object to process
	Returns:
		awards: number of awards and nominations for an actor
	"""
	awards = soup.find("span", class_="page__headerCounter")
	# Actor may have no awards nor any nominations
	awards = awards.string.strip("()") if awards else 0

	return awards

def dct_to_csv(dct: dict, filename: str, columns: typing.Iterable=None) -> None:
	"""Helper function mapping the dictionary to csv file
	Arguments:
		dct: dictionary to save to csv file
		filename: name of the new csv file
		columns: optional names to be used in file header for each of the columns

	"""
	with open(filename, "w") as fff:
		DataFrame.from_dict(dct, orient="index", columns=columns).to_csv(fff, index_label="actor name")

if __name__ == '__main__':
	FILMWEB_BASE_URL = "https://www.filmweb.pl"
	ACTORS_BASE_URL = f"{FILMWEB_BASE_URL}/persons/search?orderBy=popularity&descending=true"

	actors_pages = {}
	actors_db = {}
	with Session() as s: ## all within same TCP connection
		try:

			for url in next_actor_page(ACTORS_BASE_URL, start_page=101, max_page=250):  #scrape actors names and urls to their sub-pages
				response = s.get(url, timeout=12)
				response.encoding = "utf-8"

				soup = BeautifulSoup(response.text, "html.parser")
				actors_list = soup.find("ul", class_="resultsList hits").contents

				for li_actor in actors_list:
					actor_anchor = li_actor.find_next(a_has_h3_parent)
					actor_name = actor_anchor.string
					populate_dct(actor_name, actors_pages, actor_anchor["href"].strip())

				#cache_response(response.text, f"actors_page_{actor_name}")
				sleep(1) # sleep between requests not to overload the server
		except Exception as e:
			print(e)
		finally:
			dct_to_csv(actors_pages, "pages_for_actor101-250.csv")  # save in order to minimize requests in case of failures

		try:

			for actor_name, actor_url in actors_pages.items():  # scrape each actors specific parameters
				response_info = s.get(FILMWEB_BASE_URL+actor_url[0], timeout=12)
				response_info.encoding = "utf-8"
				response_awards = s.get(FILMWEB_BASE_URL+actor_url[0]+"/awards", timeout=12)
				response_awards.encoding = "utf-8"
				# cache_response(response_info.text, f"details_{actor_name}")
				# cache_response(response_awards.text, f"awards_{actor_name}")

				soup_details = BeautifulSoup(response_info.text, "html.parser")
				soup_awards = BeautifulSoup(response_awards.text, "html.parser")
				rating, votes = get_actor_rating(soup_details)
				awards = get_actor_awards(soup_awards)
				populate_dct(actor_name, actors_db, rating, votes, awards)

				sleep(1)  # sleep between requests
		except Exception as e:
			print(e)
		finally:
			columns = ["rating", "votes", "awards"]
			dct_to_csv(actors_db, "filmweb_actors101_250.csv", columns=columns)

	# for k,v in actors_pages.items():  # DEBUG STUFF
	# 	print(k ,v )
	#
	# for k, v in actors_db.items():
	# 	print(k, v)