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

# Function to check if a URL is valid for HTTP requests
def is_valid_url(url):
    return url.startswith("http")

# Function to remove common navigation elements
def remove_unwanted_elements(soup):
    # Remove common navigation sections
    for tag in ['nav', 'header', 'footer', 'aside']:
        for element in soup.find_all(tag):
            element.decompose()

    # Remove specific elements based on class or id
    for class_name in ['navbar', 'footer', 'sidebar', 'header']:
        for element in soup.find_all(class_=class_name):
            element.decompose()

# Function to clean text: remove tabs and normalize newlines
def clean_text(text):
    # Remove tabs and replace with a single space
    text = re.sub(r'\t', ' ', text)
    # Replace multiple newlines with a single space
    text = re.sub(r'\n+', '\n', text)
    # Trim leading and trailing spaces
    return text.strip()

# Function to scrape file contents from a URL
def scrape_files(url, output_file, max_files=100000, char_limit=10000):
    files_scraped = 0
    visited = set()
    already_scraped = set()

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

                    # Remove unwanted elements
                    remove_unwanted_elements(soup)

                    # Get all text and clean it
                    plain_text = clean_text(soup.get_text())

                    output.write(f"URL: {url}\n")
                    output.write(f"Content:\n{plain_text}\n")
                    output.write("-" * 20 + "\n\n")
                    output.flush()

                    files_scraped += 1

                    # Continue to the next links on the same page
                    for link in soup.find_all('a', href=True):
                        href = link['href']
                        if is_valid_url(href):
                            absolute_url = urljoin(url, href)

                            if absolute_url in already_scraped:
                                continue

                            already_scraped.add(absolute_url)
                            scrape_recursive(absolute_url)

                else:
                    print(f"Failed to fetch {url}: {response.status_code}")
            except Exception as e:
                print(f"Error fetching {url}: {e}")

        scrape_recursive(url)

# URL to start scraping from
start_url = 'https://www.sec.gov'

# Output file to store the content snippets
output_file = 'scraped_content.txt'

scrape_files(start_url, output_file, max_files=100000, char_limit=10000)
