require 'open-uri'
require 'net/http'
require 'httparty'
require 'json' # useful if data comes in json format
require 'nokogiri'
require 'pry'
require 'csv'

# url = "https://en.wikipedia.org/wiki/Hydrogen"
# uri = URI.parse(url)
# response = Net::HTTP.get_response(uri)
# puts response.body


#html = HTTParty.get("https://en.wikipedia.org/wiki/Hydrogen")

# JSON.parse response, symbolize_names: true

html = open("https://en.wikipedia.org/wiki/Hydrogen")
doc = Nokogiri::HTML(html)

description = doc.css("p").text.split("\n").find{|e| e.length > 0}

dataArr = []

dataArr.push([description])

CSV.open('data.csv', "w") do |csv|
	csv << dataArr
end