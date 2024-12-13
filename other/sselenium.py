from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.chrome.service import Service as ChromeService
from selenium.webdriver.common.by import By
from bs4 import BeautifulSoup
import re
import time

# Set up Selenium with headless Chrome
chrome_options = Options()
chrome_options.add_argument("--headless")  # Run in headless mode
chrome_options.add_argument("--disable-gpu")
chrome_options.add_argument("--no-sandbox")
chrome_options.add_argument("--disable-dev-shm-usage")

# Path to your ChromeDriver (adjust accordingly)
chromedriver_path = "chrome.pkr"
chrome_service = ChromeService(executable_path=chromedriver_path)

# Create a new Selenium WebDriver
driver = webdriver.Chrome(service=chrome_service, options=chrome_options)

# Function to clean text
def clean_text(text):
    text = re.sub(r'\t', ' ', text)  # Replace tabs with spaces
    text = re.sub(r'\n+', ' ', text)  # Replace multiple newlines with a single space
    return text.strip()

# URL to scrape
url = 'www.twitter.com'  # Change to the target URL

# Open the page and wait for JavaScript to load
driver.get(url)
time.sleep(2)  # Allow some time for JavaScript to run

# Get the page source after JavaScript has loaded
page_source = driver.page_source

# Use BeautifulSoup to parse the loaded content
soup = BeautifulSoup(page_source, 'html.parser')

# Extract text and clean it
plain_text = clean_text(soup.get_text())

print("Scraped Content:")
print(plain_text)

# Close the Selenium WebDriver
driver.quit()
