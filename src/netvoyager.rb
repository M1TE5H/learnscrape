require 'open-uri'
require 'net/http'
require 'httparty'
require 'json' # useful if data comes in json format
require 'nokogiri'
require 'pry'
require 'csv'
# require 'kimurai'

# url = "https://en.wikipedia.org/wiki/Hydrogen"
# uri = URI.parse(url)
# response = Net::HTTP.get_response(uri)
# puts response.body


#html = HTTParty.get("https://en.wikipedia.org/wiki/Hydrogen")

# JSON.parse response, symbolize_names: true



# Static Page Scraper
# class DensityScraper < Kimurai::Base
# 	@name = "density_scraper"
# 	@start_urls = []
# 	@engine = :selenium_chrome

# 	@@densities = []

# 	def scrape_page
# 		doc = browser.current_response
# 		returned_jobs = doc.css('')
# 	end

# 	def parse(response, url:, data: {})
# 		scrape_page
# 		@@densities
# 	end 
# end




# ========
html = open("https://en.wikipedia.org/wiki/Chemical_element")
doc = Nokogiri::HTML(html)

description = doc.css("h2").text.split("\n")

# description = doc.css("p").text.split("\n").find{|e| e.length > 0}

dataArr = []

dataArr.push([description])

CSV.open('data.csv', "w") do |csv|
	csv << dataArr
end