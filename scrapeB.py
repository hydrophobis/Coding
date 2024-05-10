import requests
from bs4 import BeautifulSoup
from urllib.parse import urljoin
import re

# Custom headers to mimic browser behavior
headers = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36',
    'Accept-Language': 'en-US,en;q=0.5',
    'Accept-Encoding': 'gzip, deflate, br',
    'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8',
    'Connection': 'keep-alive'
}

# Function to clean text: remove tabs and normalize newlines
def clean_text(text):
    # Remove tabs and replace with a single space
    text = re.sub(r'\t', ' ', text)
    # Replace multiple newlines with a single space
    text = re.sub(r'\n+', ' ', text)
    # Trim leading and trailing spaces
    return text.strip()

# Function to scrape content from a URL and follow links
def scrape_files(url, output_file, max_files=1000, char_limit=10000):
    files_scraped = 0
    visited = set()

    with open(output_file, 'w') as output:
        def scrape_recursive(url):
            nonlocal files_scraped
            if files_scraped >= max_files:
                return

            if url in visited:
                return
            visited.add(url)

            try:
                response = requests.get(url, headers=headers)
                if response.status_code == 200:
                    soup = BeautifulSoup(response.content, 'html.parser')

                    # Clean and extract text from the page
                    plain_text = clean_text(soup.get_text())
                    output.write(f"URL: {url}\n")
                    output.write(f"Content:\n{plain_text}\n")
                    output.write("-" * 20 + "\n")
                    output.flush()

                    files_scraped += 1

                    # Follow links to scrape new pages
                    for link in soup.find_all('a', href=True):
                        href = link['href']
                        if href and href.startswith('/'):
                            absolute_url = urljoin(url, href)
                            scrape_recursive(absolute_url)

                else:
                    print(f"Failed to fetch {url}: {response.status_code}")
            except Exception as e:
                print(f"Error fetching {url}: {e}")

        scrape_recursive(url)

# URL to start scraping from
start_url = 'https://ebay.com/'

# Output file to store the content snippets
output_file = 'scraped_content.txt'

# Start scraping from the initial URL
scrape_files(start_url, output_file, max_files=100000, char_limit=1000000)
